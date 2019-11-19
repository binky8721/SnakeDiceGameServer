#pragma once
#include <iostream>
#include <WinSock2.h>
using namespace std;

// ���� ��� ���� ����ü
struct ClientRecord
{
	int winCount;
	int loseCount;
};

enum E_GameState
{
	LoginState,
	Lobby,
	InGame,
	Result
};

class ClientInfo
{
	int keyID;	// key id: ���ӿ� �������϶��� �ĺ��� ���� ����ϴ� ������
	string clientID;	// id
	string clientPW;	// pw
	string clientIcon;	// imoticon
	ClientRecord record;	// win - lose record
	SOCKET mySock;

	E_GameState clientGameState;

public:
	ClientInfo();
	ClientInfo(int keyID, string id, string pw, string icon, int winCout, int loseCount, SOCKET mySock);
	~ClientInfo();

	int GetKeyID()const;
	string GetClientID()const;
	SOCKET GetMySock() const;
};

