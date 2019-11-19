#include"ClientCommunicator.h"
#include "Packet.h"
#include "LoginManager.h"
#include "GameManager.h"
using namespace std;
enum ErrorCode
{
	InitializeFail,
	ConnectFail,
};

int main(int argc, char *argv[])
{
	try
	{
		if (ClientCommunicator::GetInstance()->Init() == false)
			throw InitializeFail;

		if (ClientCommunicator::GetInstance()->Connect() == false)
			throw ConnectFail;
	}
	catch (ErrorCode code)
	{
		switch (code)
		{
		case InitializeFail:
			cout << " Socket initialize failed!" << endl;
			return -1;

		case ConnectFail:
			cout << "Connetion failed!" << endl;
			return -1;

		default:
			cout << "Unknown Error" << endl;
			return -1;
		}
	}

	/*			뱀 사다리 게임 규칙 UI				*/
	cout << "							< 뱀 사다리 게임 규칙 >							" << endl;
	cout << "****************************************************************************" << endl;
	cout << " 1. 플레이어는 본인의 차례가 되면 주사위를 굴려 눈금만큼 이동합니다.		" << endl;
	cout << " 2. 도착한 지점에 뱀의 꼬리가 있다면, 그 뱀의 머리가 있는 칸으로 이동합니다." << endl;
	cout << " 3. 결승 지점에 먼저 도착한 플레이어가 게임에서 승리합니다.				" << endl;
	cout << "****************************************************************************" << endl;
	char buf[BUFSIZE];
	// game loop
	ResponsePacket response;
	response.header = Response;
	SendGameInfoPacket SendInfo;
	SendInfo.header = SendGameInfo;
	RecvGameInfoPacket RecvInfo;
	RecvInfo.header = RecvGameInfo;
	MatchingSuccessPacket matching;
	string chat;
	int prevPos = 0;
	string prevName = matching.id;

	while (1)
	{
		E_GameState state = GameManager::GetInstance()->GetGameState();
		switch (state)
		{
		case LoginState:
			if (LoginManager::GetInstance()->GetInputType() == None)	// 선택된 로그인 타입이 없으면
				LoginManager::GetInstance()->SelectInputType();	// 로그인 타입 선택

			//LoginManager::GetInstance()->InputLoginInfo();	// 선택한 로그인 타입으로 진행
			if (LoginManager::GetInstance()->StartLoginProcess(*ClientCommunicator::GetInstance()))
			{

				response.isSuccess = false;
				response.errMessage = "";

				//ZeroMemory(&response, sizeof(ResponsePacket));
				//int retVal =  ClientCommunicator::GetInstance()->Recvn(&response, sizeof(ResponsePacket)); // 서버로부터의 응답
				ZeroMemory(buf, BUFSIZE);
				int retVal = ClientCommunicator::GetInstance()->Recvn(buf, sizeof(ResponsePacket));	// 서버로부터의 응답
				memcpy(&response, buf, sizeof(ResponsePacket));
				if (!response.isSuccess)
				{
					if (response.errMessage != "")
						cout << response.errMessage << endl;

					//로그인 실패시 다시 로그인시도
					continue;
					//return -1;
				}
				else
				{
					SuccessLoginPacket successLogin;
					ZeroMemory(buf, BUFSIZE);
					int retVal = 0;
					retVal = ClientCommunicator::GetInstance()->Recvn(buf, sizeof(SuccessLoginPacket));	// 로그인이 성공하면 로비로 이동하라는 서버의 패킷이 날아온다
					memcpy(&successLogin, buf, sizeof(ResponsePacket));
					GameManager::GetInstance()->SetGameState((E_GameState)successLogin.gameState);
				}
			}
			else
			{
				// 로그인 실패: 다시시도
			}

			break;

		case Lobby:
		{
			// 상대방 대기 프로세스(큐가 잡혔다는것을 서버쪽에서 보내줘야함)
			cout << "상대방을 기다리는 중입니다...." << endl;
			int ret = 0;
			ret = ClientCommunicator::GetInstance()->Recvn(buf, sizeof(MatchingSuccessPacket));
			if (ret == SOCKET_ERROR)
				return -1;

			memcpy(&matching, buf, sizeof(MatchingSuccessPacket));
			cout << "매칭성공, 상대방: " << matching.id << endl;
			GameManager::GetInstance()->SetGameState((E_GameState)matching.gameState);

		}

		break;

		case InGame:
		{
			// 게임 실행
			ZeroMemory(buf, BUFSIZE);
			int ret = 0;
			ret = ClientCommunicator::GetInstance()->Recvn(buf, sizeof(RecvGameInfoPacket));
			if (ret == SOCKET_ERROR) {
				cout << "fail to recv packet" << endl;
				continue;
			}
			memcpy(&RecvInfo, buf, sizeof(RecvGameInfoPacket));
			
			cout << "\nID: " << RecvInfo.id << endl;
			cout << "chat: " << RecvInfo.chat << endl;
			cout << RecvInfo.changePosChat;
			string message = RecvInfo.isMyTurn ? "yes" : "no";
			cout << "is My Turn? :" << message << endl;
			cout << "Total pos(" << RecvInfo.pos << ")" << endl;
			cout << "posX :" << RecvInfo.posX << "\tposY : " << RecvInfo.posY << endl;
			cout << ", dice(" << RecvInfo.dice << ")" << endl;
			
			if (prevPos < RecvInfo.pos && matching.id == RecvInfo.id && prevName != RecvInfo.id) {
				prevName = RecvInfo.id;
				cout <<RecvInfo.id<< "가 추월했습니다." << endl;
			}

			// 내턴이 아니면 continue 같은 대기상태가 필요하다
			if (RecvInfo.isGameOver == true) {
				cout << "\nwinner is " << RecvInfo.id << endl;
				state = Result;
				break;
			}
			prevPos = RecvInfo.pos;
			if (!RecvInfo.isMyTurn) {
				cout << "상대방 차례입니다." << endl;
				continue;
			}
				
			cout << "채팅을 입력하세요." << endl;
			cin.ignore(); //이전에 입력버퍼에 받아져있던 개행을 지우기위해 입력버퍼를 비워줌
			getline(cin, chat); //그후 getline사용
			cin.putback('\n');
			/*cin >> chat;*/
			SendInfo.chat = chat;
			ZeroMemory(buf, BUFSIZE);

			memcpy(buf, &SendInfo, sizeof(SendGameInfoPacket));
			if (!ClientCommunicator::GetInstance()->Send(buf, sizeof(SendGameInfoPacket))) {
				cout << "fail to send packet" << endl;
				continue;
			}
		}
			break;

		case Result:
			// 결과 출력 후 로비로 이동
			break;
		}
	}

	//	/*							*/
	//	cout << "** 게임 내에서 사용할 아바타를 골라주세요. (1, 2, 3, 4 중 하나 입력) **" << endl;
	//	cout << "	1. U('x')U		2. /('(00)')/		3. / ('<>') |		4. <')))><<	" << endl;
	//	cout << "****************************************************************************" << endl;
	//	/*		구현 성공하면 LoginHeader에 avatar 추가해서 아바타 받기		*/
	//	/*ZeroMemory(temp, sizeof(temp));
	//	cout << "사용할 아바타 번호를 입력해주세요. : ";
	//	if (fgets(temp, 5, stdin) == NULL)
	//		return 0;

	//	temp[strlen(temp) - 1] = '\0';
	//	LoginHeader.avatar = temp;*/

	return 0;
}