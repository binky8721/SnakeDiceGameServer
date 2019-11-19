#pragma once
#include <iostream>
#include <map>
#include "ClientInfo.h"
#include "Server.h"
#include "Board.h"
#include "Player.h"
#include "Packet.h"

using namespace std;

enum E_PlayerType
{
	First,
	Secont,
};

// �ΰ����� �����ϴ� Ŭ����, �� ���� �־���ϹǷ� �̱����� �ƴ�
class InGameManager
{
	map<int, ClientInfo*> clientDic;
	int inGameID;
	int nowTurn;
	// todo �� ����, �������� ���
	bool isInit;
	bool isGameOver;
	Board* b;
	SendGameInfoPacket SendPlayer1Info;
	SendGameInfoPacket SendPlayer2Info;
	RecvGameInfoPacket RecvInfo;
	Player* p1;
	Player* p2;
	int playerKey[2];

public:
	InGameManager();
	~InGameManager();

	int InitBoard();
	void ReleaseBoard();

	void SetInGameID(int id);
	int GetInGameID() const;

	int GetNowTurn() const;
	bool GetIsInit() const;
	bool GetIsGameOver() const;

	int CreateInGameId();
	bool IsContainKey(int key);
	string GetOpponentId(int myKey);

	bool GameProcess(int key);

	void SetClientDic(ClientInfo *first, ClientInfo *second);	// ���� ������ �濡 �ִ� Ŭ���̾�Ʈ���� ������ ����

	int PosToX(int position);
	int PosToY(int position);
	string IsChangePos(int position);
};

