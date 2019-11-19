#include "ClientCommunicator.h"

ClientCommunicator* ClientCommunicator::instance = nullptr;
ClientCommunicator::ClientCommunicator()
{
}


ClientCommunicator* ClientCommunicator::GetInstance()
{
	if (instance == nullptr)
		instance = new ClientCommunicator();

	return instance;
}

ClientCommunicator::~ClientCommunicator()
{
}

bool ClientCommunicator::Init()
{
	// 윈속 초기화	
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) { 
		err_quit("socket()"); 
		return false;
	}

	// connect()
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);

	hostent* host = gethostbyname(getenv("COMPUTERNAME"));
	ip.s_addr = *((u_long *)(host->h_addr_list[0]));
	strcpy(ip_buf, inet_ntoa(ip));

	return true;
}

bool ClientCommunicator::Connect()
{
	int retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) { 
		err_quit("connect()"); 
		return false;
	}

	//gameState = LoginState;
	return true;
}

bool ClientCommunicator::Send(char * data, int len)
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

bool ClientCommunicator::Recv()
{
	int retval = recv(sock, (char*)&share, sizeof(Share), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		return false;
	}
	else if (retval == 0)
		return false;

	return true;
}

int ClientCommunicator::Recvn(char*buf, int len)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(sock, ptr, left, 0);
		if (received == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if (received == 0)
		{
			break;
		}

		left -= received;
		ptr += received;
	}

	return (len - left);
}

//int ClientCommunicator::Recvn(PacketType* packet, int len)
//{
//	int received;
//	char* ptr = buf;
//	int left = len;
//
//	while (left > 0)
//	{
//		received = recv(sock, buf, left, 0);
//		if (received == SOCKET_ERROR)
//		{
//			return SOCKET_ERROR;
//		}
//		else if (received == 0)
//		{
//			break;
//		}
//
//		left -= received;
//		ptr += received;
//	}
//
//	
//	memcpy(&packet, buf, received);
//
//	return (len - left);
//}

const Share ClientCommunicator::GetShare() {
	return share;
}

void ClientCommunicator::AddShareValue(int data) {
	share.share += data;
}

char* ClientCommunicator::GetIPBuf() {
	return ip_buf;
}

void ClientCommunicator::Release()
{
	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
}

void ClientCommunicator::err_quit(const char * msg)
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

void ClientCommunicator::err_display(const char * msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}