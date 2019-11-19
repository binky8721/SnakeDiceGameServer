#include "server.h"


Server* Server::instance = nullptr;
Server::Server()
{
}

Server * Server::GetInstace()
{
	if (instance == nullptr)
		instance = new Server();

	return instance;
}

Server::~Server()
{
}

bool Server::Init()
{
	// ���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	// socket()
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) {
		err_quit("socket()");
		return false;
	}

	// bind()
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	return true;
}

bool Server::Bind()
{
	int retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		err_quit("bind()");
		return false;
	}

	return true;
}

bool Server::Listen()
{
	int retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		err_quit("listen()");
		return false;
	}

	return true;
}

bool Server::Accept()
{
	addrlen = sizeof(clientaddr);
	client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
	if (client_sock == INVALID_SOCKET) {
		err_display("accept()");
		return false;
	}

	// ù ���ӽ� �α� ���
	std::string unknown = "UnKnownUser";
	LogManager::GetInstance()->UserAccessLog(unknown);

	// ������ Ŭ���̾�Ʈ ���� ���
	printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	// ������ ����
	hThread = CreateThread(NULL, 0, ProcessGame,
		(LPVOID)this, 0, NULL);
	if (hThread == NULL) { closesocket(client_sock); }
	else {
		CloseHandle(hThread);
	}

	return true;
}

DWORD __stdcall Server::ProcessClient(LPVOID arg)
{
	//Server* temp = (Server*)arg;
	//SOCKET client_sock = temp->client_sock;
	//int retval;
	//SOCKADDR_IN clientaddr;
	//int addrlen;
	//char buf[BUFSIZE + 1];

	//RegisterPacket RegisterHeader;

	//// Ŭ���̾�Ʈ ���� ���
	//addrlen = sizeof(clientaddr);
	//getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);
	//// ������ �ޱ�
	//retval = recv(client_sock, buf, BUFSIZE, 0);
	//if (retval == SOCKET_ERROR) {
	//	temp->err_display("recv()");
	//}

	//memcpy(&RegisterHeader, buf, sizeof(RegisterPacket));

	//// ���� ������ ���
	//buf[retval] = '\0';
	//printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
	//	ntohs(clientaddr.sin_port), buf);
	//strcpy(temp->share.ip, buf);

	//while (1) {
	//	// ������ ������
	//	retval = send(client_sock, (char*)&temp->share, sizeof(Share), 0);
	//	if (retval == SOCKET_ERROR) {
	//		temp->err_display("send()");
	//		break;
	//	}
	//	int len = sizeof(Share);
	//	Share tmpShare;
	//	retval = recv(client_sock, (char*)&tmpShare, sizeof(Share), 0);
	//	if (retval == SOCKET_ERROR) {
	//		temp->err_display("recv()");
	//		break;
	//	}
	//	else if (retval == 0)
	//		break;

	//	EnterCriticalSection(&temp->cs);
	//	temp->share = tmpShare;
	//	LeaveCriticalSection(&temp->cs);
	//	printf("share = %d\n", temp->share.share);
	//}

	//// closesocket()
	//closesocket(client_sock);
	//printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
	//	inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}

DWORD __stdcall Server::ProcessGame(LPVOID arg)
{
	
	//Server* temp = (Server*)arg;	// todo �ʿ�x
	SOCKET client_sock = Server::GetInstace()->GetClientSock();
	int retval = 0;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];
	// gameManager, logManager, loginManager

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);


	/**************************  ������ �κ�  ******************************/
	LoginPacket loginPacket;
	MatchingSuccessPacket matchingSuccessPacket;
	ClientInfo* thisClient = new ClientInfo();
	E_GameState gameState = LoginState;
	//InGameManager* ingameMgr = new InGameManager();
	while (true)
	{
		switch (gameState)
		{
		case LoginState:
		{
			if (LoginManager::GetInstance()->StartLoginProcess(client_sock, *thisClient))
			{
				gameState = Lobby;
				continue;
			}
			return -1;//Ŭ���̾�Ʈ�� ������ ���ο� �����带 �����ؾ��ϱ� ����
		}

		case Lobby: //��Ī
		{
			//EnterCriticalSection(&cs);
			if (!GameManager::GetInstance()->StartGameMatching(thisClient))
				break;
			//LeaveCriticalSection(&cs);
			// Ŭ�� ��Ŷ�� ������ ���� ������Ʈ ���� ��û
			matchingSuccessPacket.id = GameManager::GetInstance()->GetOpponentId(thisClient->GetKeyID());
			matchingSuccessPacket.gameState = (int)InGame;
			ZeroMemory(buf, sizeof(MatchingSuccessPacket));
			memcpy(buf, &matchingSuccessPacket, sizeof(matchingSuccessPacket));
			if (Server::GetInstace()->Send(client_sock, buf, sizeof(MatchingSuccessPacket)))
				gameState = InGame;
		}
		break;

		case InGame:
			EnterCriticalSection(&cs);
			if (GameManager::GetInstance()->GetIsInit(thisClient->GetKeyID()) == false) {
				cout << "InitBoard: " << thisClient->GetClientID() << endl;
				GameManager::GetInstance()->InitBoard(thisClient->GetKeyID());
			}
			if (GameManager::GetInstance()->GameProcess(GameManager::GetInstance()->GetNowTurn(thisClient->GetKeyID())) == false) {
				ZeroMemory(buf, sizeof(PacketType));
				memcpy(buf, &matchingSuccessPacket, sizeof(PacketType));
				if (!Server::GetInstace()->Send(client_sock, buf, sizeof(PacketType))) {
					cout << GameManager::GetInstance()->GetIsInit(thisClient->GetKeyID()) << endl;
					cout << "send error" << endl;
				}
				cout << GameManager::GetInstance()->GetIsInit(thisClient->GetKeyID()) << endl;
				cout << "GameProcess Fail" << endl;
				LeaveCriticalSection(&cs);
				return -1;
			}
			/*if (GameManager::GetInstance()->GetNowTurn(thisClient->GetKeyID()) == thisClient->GetKeyID())
				GameManager::GetInstance()->GameProcess(GameManager::GetInstance()->GetNowTurn(thisClient->GetKeyID()));*/
			if (GameManager::GetInstance()->GetIsGameOver(thisClient->GetKeyID()) == true) {
				gameState = Result;
			}
			break;
		case Result:
			GameManager::GetInstance()->ReleaseBoard(thisClient->GetKeyID());
			return 0;
		}

	}

#pragma region  Obsolete
	//// ������ �ޱ�
	//retval = recv(client_sock, buf, BUFSIZE, 0);
	//if (retval == SOCKET_ERROR) {
	//	Server::GetInstace()->err_display("recv()");
	//}

	//// ���� ������ ���
	//buf[retval] = '\0';
	//printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
	//	ntohs(clientaddr.sin_port), buf);
	//strcpy(Server::GetInstace()->share.ip, buf);

	//while (1) {
	//	// ������ ������
	//	retval = send(client_sock, (char*)&Server::GetInstace()->share, sizeof(Share), 0);
	//	if (retval == SOCKET_ERROR) {
	//		Server::GetInstace()->err_display("send()");
	//		break;
	//	}
	//	int len = sizeof(Share);
	//	Share tmpShare;
	//	retval = recv(client_sock, (char*)&tmpShare, sizeof(Share), 0);
	//	if (retval == SOCKET_ERROR) {
	//		Server::GetInstace()->err_display("recv()");
	//		break;
	//	}
	//	else if (retval == 0)
	//		break;
	//}

	//closesocket(client_sock);

	//// todo �αױ��
	//printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
	//	inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
#pragma endregion


	//  ��������ɶ� gamemanager connectedUserCount --
	DeleteCriticalSection(&cs);
	return 0;
}

void Server::Release()
{
	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
}

bool Server::Send(SOCKET sock, char * data, int len)
{
	int retval = send(sock, data, len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return false;
	}
	else if (retval == 0)
		return false;
	return true;
}

int Server::Recvn(SOCKET sock, char * data, int len)
{
	int received = 0;
	received = recv(sock, data, len, 0);
	if (received == SOCKET_ERROR)
	{
		err_display("recv()");
		return SOCKET_ERROR;
	}
	else if (received == 0)
	{
		return 0;
	}

	return received;
}

SOCKET Server::GetClientSock() const
{
	return client_sock;
}

void Server::err_quit(const char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
}

void Server::err_display(const char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	printf("[%s] %s", msg, (char*)lpMsgBuf);

	LocalFree(lpMsgBuf);
}
