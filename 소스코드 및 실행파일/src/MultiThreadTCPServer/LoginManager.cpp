#include "LoginManager.h"

LoginManager* LoginManager::instance = nullptr;


LoginManager* LoginManager::GetInstance()
{
	if (instance == nullptr)
		instance = new LoginManager();

	return instance;
}

LoginManager::LoginManager()
{
	userInfoFileName = "UserInfo.txt";
}

LoginManager::~LoginManager()
{
}

bool LoginManager::StartLoginProcess(SOCKET clientSock, ClientInfo& thisClient)
{
	PacketType parentPacket;
	char tempBuf[BUFSIZE];
	ZeroMemory(tempBuf, BUFSIZE);
	int retVal = Server::GetInstace()->Recvn(clientSock, tempBuf, BUFSIZE);	// 받을수 있는 만큼 최대한 받는다.
	if (retVal == SOCKET_ERROR)
	{
		LogManager::GetInstance()->UserDisconnectLog(thisClient.GetClientID());
		cout << "socket error" << endl;
		return false;
	}

	memcpy(&parentPacket, tempBuf, sizeof(PacketType));

	ResponsePacket response;
	response.header = Response;
	/*LoginPacket loginPacket;
	RegisterPacket registerPacket;*/
	try
	{
		switch (parentPacket.header)
		{
		case Register:
		{
			
			//todo 유저 디비 검색해서 아이디 등록이 가능한지 체크(불가능 하면 false 리턴)
			memcpy(&registerPacket, tempBuf, parentPacket.totalSize);
			if (retVal == SOCKET_ERROR)
				throw SocketError;

			//if (!(UserDBManager::GetInstance()->IsCanRegisterInfo(registerPacket.id,registerPacket.pw)))	// TODO 검증 작업 완료되면 작업하기
			//	throw AleadyExistUser;
			int registerState = UserDBManager::GetInstance()->IsCanRegisterInfo(registerPacket.id, registerPacket.pw);
			
			switch (registerState)
			{
			case 1: //성공
				break;
			case 2: //중복
				throw AleadyExistUser;
			case 3: //ID길이 제한
				throw TooLongUserID;
			case 4: //PW길이 제한
				throw TooLongUserPW;
			}

			//회원가입 정보 저장
			RegisterInfoSave(registerPacket);
			GameManager::GetInstance()->UserLogin(GetKeyID(), registerPacket.id, registerPacket.pw, "", thisClient, clientSock);
		}
		break;

		case Login:
		{
			
			// todo 유저 디비 검색해서 아이디 비번이 있는지 체크(불가능하면 false 리턴)
			memcpy(&loginPacket, tempBuf, parentPacket.totalSize);
			int loginState = UserDBManager::GetInstance()->IsCanLoginInfo(loginPacket.id, loginPacket.pw);
			if (loginState == 1)
				throw WrongUserId;
			else if (loginState == 2)
				throw WrongPassword;
			//todo 검증 작업 완료되면..
			int key = GetKeyID();
			if (key == -1)
				throw Unknown;

			std::cout << "로그인 성공!" << std::endl;
			LogManager::GetInstance()->UserAccessLog(loginPacket.id); //접속 로그 기록
			GameManager::GetInstance()->UserLogin(key,loginPacket.id, loginPacket.pw, "",thisClient, clientSock);
			 
		}
		break;

		default:
			throw WrongPacketType;
			break;
		}
	}
	catch (E_ExceptionType type)
	{
		string errMessage = "";
		switch (type)
		{
		case TooLongUserID:
			errMessage = "TooLongUserID";
			break;

		case TooLongUserPW:
			errMessage = "TooLongUserPW";
			break;

		case AleadyExistUser:
			errMessage = "AleadyExistUser";
			break;

		case WrongPacketType:
			errMessage = "WrongPacketType";
			break;

		case SocketError:
			errMessage = "SocketError";
			break;

		case WrongPassword:
			errMessage = "WrongPassword";
			break;

		case WrongUserId:
			errMessage = "WrongUserId";
			break;

		default:
			errMessage = "Unknown error";
			break;
		}
		cout << errMessage << endl;
		response.isSuccess = false;
		response.errMessage = errMessage;
		if (!Server::GetInstace()->Send(clientSock, (char*)&response, sizeof(ResponsePacket)))
		{
			cout << "fail to send response packet" << endl;
			return false;
		}

		
		return false;
	}
	
	response.isSuccess = true;	// 클라에 결과값 보내는 코드
	if (!Server::GetInstace()->Send(clientSock, (char*)&response, sizeof(ResponsePacket)))
	{
		cout << "fail to send response packet" << endl;
		return false;
	}

	SuccessLoginPacket successLogin;
	successLogin.gameState = (int)Lobby;
	successLogin.header = SuccessLogin;
	successLogin.totalSize = sizeof(SuccessLoginPacket);
	ZeroMemory(tempBuf, BUFSIZE);
	memcpy(tempBuf, &successLogin, sizeof(SuccessLoginPacket));
	if (!Server::GetInstace()->Send(clientSock, tempBuf, successLogin.totalSize))
	{
		cout << "fail to send SuccessLogin packet" << endl;
		return false;
	}
	
	

	return true;
}

void LoginManager::RegisterInfoSave(const RegisterPacket &rp)
{
	std::ofstream file(userInfoFileName, std::ios_base::out | std::ios_base::app);
	file << rp.id << "#" << rp.pw << "#" << "\n";
	if (file.is_open())
		file.close();

}

int LoginManager::GetKeyID()
{
	int key = -1;
	int count = 0;
	srand((unsigned int)time(NULL));
	do
	{
		key = rand() % BUFSIZE;
		if (count >= BUFSIZE)
			return key;

		count++;
	} while (GameManager::GetInstance()->IsContainedKey(key));
	return key;
}
