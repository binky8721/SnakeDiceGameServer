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

// 인게임을 관리하는 클래스, 방 마다 있어야하므로 싱글톤이 아님
class InGameManager
{
	map<int, ClientInfo*> clientDic;
	int inGameID;
	int nowTurn;
	// todo 맵 정보, 유저정보 등등
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

	void SetClientDic(ClientInfo *first, ClientInfo *second);	// 게임 시작전 방에 있는 클라이언트들의 정보를 저장

	int PosToX(int position);
	int PosToY(int position);
	string IsChangePos(int position);
};

