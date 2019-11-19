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

	/*			�� ��ٸ� ���� ��Ģ UI				*/
	cout << "							< �� ��ٸ� ���� ��Ģ >							" << endl;
	cout << "****************************************************************************" << endl;
	cout << " 1. �÷��̾�� ������ ���ʰ� �Ǹ� �ֻ����� ���� ���ݸ�ŭ �̵��մϴ�.		" << endl;
	cout << " 2. ������ ������ ���� ������ �ִٸ�, �� ���� �Ӹ��� �ִ� ĭ���� �̵��մϴ�." << endl;
	cout << " 3. ��� ������ ���� ������ �÷��̾ ���ӿ��� �¸��մϴ�.				" << endl;
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
			if (LoginManager::GetInstance()->GetInputType() == None)	// ���õ� �α��� Ÿ���� ������
				LoginManager::GetInstance()->SelectInputType();	// �α��� Ÿ�� ����

			//LoginManager::GetInstance()->InputLoginInfo();	// ������ �α��� Ÿ������ ����
			if (LoginManager::GetInstance()->StartLoginProcess(*ClientCommunicator::GetInstance()))
			{

				response.isSuccess = false;
				response.errMessage = "";

				//ZeroMemory(&response, sizeof(ResponsePacket));
				//int retVal =  ClientCommunicator::GetInstance()->Recvn(&response, sizeof(ResponsePacket)); // �����κ����� ����
				ZeroMemory(buf, BUFSIZE);
				int retVal = ClientCommunicator::GetInstance()->Recvn(buf, sizeof(ResponsePacket));	// �����κ����� ����
				memcpy(&response, buf, sizeof(ResponsePacket));
				if (!response.isSuccess)
				{
					if (response.errMessage != "")
						cout << response.errMessage << endl;

					//�α��� ���н� �ٽ� �α��νõ�
					continue;
					//return -1;
				}
				else
				{
					SuccessLoginPacket successLogin;
					ZeroMemory(buf, BUFSIZE);
					int retVal = 0;
					retVal = ClientCommunicator::GetInstance()->Recvn(buf, sizeof(SuccessLoginPacket));	// �α����� �����ϸ� �κ�� �̵��϶�� ������ ��Ŷ�� ���ƿ´�
					memcpy(&successLogin, buf, sizeof(ResponsePacket));
					GameManager::GetInstance()->SetGameState((E_GameState)successLogin.gameState);
				}
			}
			else
			{
				// �α��� ����: �ٽýõ�
			}

			break;

		case Lobby:
		{
			// ���� ��� ���μ���(ť�� �����ٴ°��� �����ʿ��� ���������)
			cout << "������ ��ٸ��� ���Դϴ�...." << endl;
			int ret = 0;
			ret = ClientCommunicator::GetInstance()->Recvn(buf, sizeof(MatchingSuccessPacket));
			if (ret == SOCKET_ERROR)
				return -1;

			memcpy(&matching, buf, sizeof(MatchingSuccessPacket));
			cout << "��Ī����, ����: " << matching.id << endl;
			GameManager::GetInstance()->SetGameState((E_GameState)matching.gameState);

		}

		break;

		case InGame:
		{
			// ���� ����
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
				cout <<RecvInfo.id<< "�� �߿��߽��ϴ�." << endl;
			}

			// ������ �ƴϸ� continue ���� �����°� �ʿ��ϴ�
			if (RecvInfo.isGameOver == true) {
				cout << "\nwinner is " << RecvInfo.id << endl;
				state = Result;
				break;
			}
			prevPos = RecvInfo.pos;
			if (!RecvInfo.isMyTurn) {
				cout << "���� �����Դϴ�." << endl;
				continue;
			}
				
			cout << "ä���� �Է��ϼ���." << endl;
			cin.ignore(); //������ �Է¹��ۿ� �޾����ִ� ������ ��������� �Է¹��۸� �����
			getline(cin, chat); //���� getline���
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
			// ��� ��� �� �κ�� �̵�
			break;
		}
	}

	//	/*							*/
	//	cout << "** ���� ������ ����� �ƹ�Ÿ�� ����ּ���. (1, 2, 3, 4 �� �ϳ� �Է�) **" << endl;
	//	cout << "	1. U('x')U		2. /('(00)')/		3. / ('<>') |		4. <')))><<	" << endl;
	//	cout << "****************************************************************************" << endl;
	//	/*		���� �����ϸ� LoginHeader�� avatar �߰��ؼ� �ƹ�Ÿ �ޱ�		*/
	//	/*ZeroMemory(temp, sizeof(temp));
	//	cout << "����� �ƹ�Ÿ ��ȣ�� �Է����ּ���. : ";
	//	if (fgets(temp, 5, stdin) == NULL)
	//		return 0;

	//	temp[strlen(temp) - 1] = '\0';
	//	LoginHeader.avatar = temp;*/

	return 0;
}