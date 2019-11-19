#include "InGameManager.h"

InGameManager::InGameManager()
{
	isInit = false;
	isGameOver = false;
}


InGameManager::~InGameManager()
{
}

int InGameManager::InitBoard()
{
	if (isInit == true) {
		return 0;
	}
	b = new Board();
	SendPlayer1Info.header = SendGameInfo;
	SendPlayer1Info.header = SendGameInfo;
	RecvInfo.header = RecvGameInfo;
	int i = 0;
	for (auto it = clientDic.begin(); it != clientDic.end(); it++) {
		playerKey[i] = it->first;
		++i;
	}

	string name;
	name = clientDic.at(playerKey[0])->GetClientID();
	p1 = new Player(name);
	b->AddPlayer(p1);

	name = clientDic.at(playerKey[1])->GetClientID();
	p2 = new Player(name);
	b->AddPlayer(p2);

	if (b->NextPlayer() == nullptr)
		return -1;

	SendPlayer1Info.id = clientDic.at(playerKey[0])->GetClientID();
	SendPlayer1Info.dice = 0;
	SendPlayer1Info.pos = 0;
	SendPlayer1Info.posX = PosToX(SendPlayer1Info.pos);
	SendPlayer1Info.posY = PosToY(SendPlayer1Info.pos);
	SendPlayer1Info.isMyTurn = true;
	SendPlayer1Info.isGameOver = false;
	SendPlayer1Info.chat = "throw dice";
	SendPlayer1Info.changePosChat = "";
	cout << "\nto: " << SendPlayer1Info.id << " address: " << this<< endl;
	cout <<" Player: " << SendPlayer1Info.id <<  " dice: "<< SendPlayer1Info.dice<<" posX: "<< SendPlayer1Info.posX<< " posY: " << SendPlayer1Info.posY<<" chat: " << SendPlayer1Info.chat<<" isMyTurn: "<< SendPlayer1Info.isMyTurn <<endl;
	if (!Server::GetInstace()->Send(clientDic.at(playerKey[0])->GetMySock(), (char*)&SendPlayer1Info, sizeof(SendGameInfoPacket))) {
		cout << "fail to send response packet" << endl;
		return -1;
	}
	nowTurn = playerKey[0];

	SendPlayer2Info.id = clientDic.at(playerKey[1])->GetClientID();
	SendPlayer2Info.dice = 0;
	SendPlayer2Info.pos = 0;
	SendPlayer2Info.posX = PosToX(SendPlayer2Info.pos);
	SendPlayer2Info.posY = PosToY(SendPlayer2Info.pos);
	SendPlayer2Info.isMyTurn = false;
	SendPlayer2Info.isGameOver = false;
	SendPlayer2Info.chat = "wait a minute";
	SendPlayer2Info.changePosChat = "";
	cout << "\nto: " << SendPlayer2Info.id << " address: " << this << endl;
	cout << " address: " << this << " Player: " << SendPlayer2Info.id << " dice: " << SendPlayer2Info.dice << " posX: " << SendPlayer2Info.posX<< " posY: " << SendPlayer2Info.posY << " chat: " << SendPlayer2Info.chat << " isMyTurn: " << SendPlayer2Info.isMyTurn << endl;
	if (!Server::GetInstace()->Send(clientDic.at(playerKey[1])->GetMySock(), (char*)&SendPlayer2Info, sizeof(SendGameInfoPacket))) {
		cout << "fail to send response packet" << endl;
		return -1;
	}

	isInit = true;
	return 0;
}

void InGameManager::ReleaseBoard()
{
	delete p1;
	delete p2;
	delete b;
}

void InGameManager::SetInGameID(int id)
{
	inGameID = id;
}

int InGameManager::GetInGameID() const
{
	return inGameID;
}

int InGameManager::GetNowTurn() const
{
	return nowTurn;
}

bool InGameManager::GetIsInit() const
{
	return isInit;
}

bool InGameManager::GetIsGameOver() const
{
	return isGameOver;
}

bool InGameManager::IsContainKey(int key)
{
	if (clientDic.find(key) != clientDic.end())
		return true;
	return false;
}

string InGameManager::GetOpponentId(int myKey)
{
	map<int, ClientInfo*>::iterator it;
	for (it = clientDic.begin(); it != clientDic.end(); it++)
	{
		if (it->first == myKey)
			continue;
		else
			break;
	}

	return it->second->GetClientID();
}

bool InGameManager::GameProcess(int key)
{
	LeaveCriticalSection(&cs);
	char tempBuf[BUFSIZE];
	ZeroMemory(tempBuf, BUFSIZE);
	int retVal = Server::GetInstace()->Recvn(clientDic.at(key)->GetMySock() , tempBuf, BUFSIZE);	// 받을수 있는 만큼 최대한 받는다.
	if (retVal == SOCKET_ERROR)
	{
		LogManager::GetInstance()->UserDisconnectLog(b->NextPlayer()->GetName());
		cout << "fail to recv packet" << endl;
		return false;
	}
	memcpy(&RecvInfo, tempBuf, sizeof(RecvInfo));

	Player* p = b->NextPlayer();
	int diceNumber = p->ThroughDice();
	int position = b->UpdatedPosition(p->GetPosition() + diceNumber);
	Player::Status status = p->Move(position);
	if (status == Player::Status::Win)
	{
		SendPlayer1Info.isGameOver = true;
		SendPlayer2Info.isGameOver = true;
		isGameOver = true;
	}

	SendPlayer1Info.id = p->GetName();
	SendPlayer1Info.dice = diceNumber;
	SendPlayer1Info.pos = position;
	SendPlayer1Info.posX = PosToX(SendPlayer1Info.pos);
	SendPlayer1Info.posY = PosToY(SendPlayer1Info.pos);
	SendPlayer1Info.chat = RecvInfo.chat;
	SendPlayer1Info.changePosChat = IsChangePos(SendPlayer1Info.pos);
	if (SendPlayer1Info.isMyTurn == false) {
		SendPlayer1Info.isMyTurn = true;
	}
	else {
		SendPlayer1Info.isMyTurn = false;
	}
	ZeroMemory(tempBuf, BUFSIZE);
	memcpy(tempBuf, &SendPlayer1Info, sizeof(SendPlayer1Info));
	//cout << "to: " << clientDic.at(playerKey[0])->GetClientID()<<" address: " <<this << endl;
	cout << "\nto: " << clientDic.at(playerKey[0])->GetClientID() << " address: " << this << endl;
	cout << "Player: " << SendPlayer1Info.id << " dice: " << SendPlayer1Info.dice
		<< " posX: " << SendPlayer1Info.posX << " posY: " << SendPlayer1Info.posY
		<< " chat: " << SendPlayer1Info.chat << " isMyTurn: " << SendPlayer1Info.isMyTurn << endl;
	cout << SendPlayer1Info.changePosChat;
	if (!Server::GetInstace()->Send(clientDic.at(playerKey[0])->GetMySock(), tempBuf, sizeof(SendGameInfoPacket))) {
		cout << "fail to send packet" << endl;
		return false;
	}

	SendPlayer2Info.id = p->GetName();
	SendPlayer2Info.dice = diceNumber;
	SendPlayer2Info.pos = position;
	SendPlayer2Info.posX = PosToX(SendPlayer2Info.pos);
	SendPlayer2Info.posY = PosToY(SendPlayer2Info.pos);
	SendPlayer2Info.chat = RecvInfo.chat;
	SendPlayer2Info.changePosChat = IsChangePos(SendPlayer2Info.pos);
	if (SendPlayer2Info.isMyTurn == false) {
		SendPlayer2Info.isMyTurn = true;
	}
	else {
		SendPlayer2Info.isMyTurn = false;
	}
	ZeroMemory(tempBuf, BUFSIZE);
	memcpy(tempBuf, &SendPlayer2Info, sizeof(SendPlayer2Info));
	//cout << "to: " << clientDic.at(playerKey[1])->GetClientID()<< " address: " << this << endl;
	cout << "\nto: " << clientDic.at(playerKey[1])->GetClientID() << " address: " << this << endl;
	cout <<"Player: " << SendPlayer2Info.id << " dice: " << SendPlayer2Info.dice
		<< " posX: " << SendPlayer2Info.posX << " posY: " << SendPlayer2Info.posY 
		<< " chat: " << SendPlayer2Info.chat << " isMyTurn: " << SendPlayer2Info.isMyTurn << endl;
	cout << SendPlayer2Info.changePosChat;
	if (!Server::GetInstace()->Send(clientDic.at(playerKey[1])->GetMySock(), tempBuf, sizeof(SendGameInfoPacket))) {
		cout << "fail to send packet" << endl;
		return false;
	}	
	
	if (nowTurn == playerKey[0]) {
		nowTurn = playerKey[1];
	}
	else {
		nowTurn = playerKey[0];
	}	
	
	return true;
}

void InGameManager::SetClientDic(ClientInfo *first, ClientInfo *second)
{
	clientDic.insert(make_pair(first->GetKeyID(), first));
	clientDic.insert(make_pair(second->GetKeyID(), second));
}

int InGameManager::PosToX(int position)
{
	if (position > 0)
		return (position % 10);
	else
		return 0;
}

int InGameManager::PosToY(int position)
{
	if (position >= 10)
		return (position / 10);
	else
		return 0;
}

string InGameManager::IsChangePos(int position)
{
	//뱀
	if (position == 16 || position == 40 || position == 55 || position == 77)
		return "Snake!\n";
	//사다리
	else if (position == 33 || position == 50 || position == 68 || position == 75 || position == 80 || position == 84 || position == 77 || position == 86 || position == 81 || position == 91)
		return "Ladder!\n";
	else
		return "";
}