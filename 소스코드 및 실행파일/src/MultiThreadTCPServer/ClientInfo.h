#pragma once
#include <iostream>
#include <WinSock2.h>
using namespace std;

// 승패 기록 관련 구조체
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
	int keyID;	// key id: 게임에 접속중일때만 식별을 위해 사용하는 고유값
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

