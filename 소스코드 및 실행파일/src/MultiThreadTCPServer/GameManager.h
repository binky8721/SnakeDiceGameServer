#pragma once
#include <queue>
#include <vector>
#include <map>
#include"ClientInfo.h"
#include"InGameManager.h"

using namespace std;

class InGameManager;

/// 전체적인 게임 관리 클래스
class GameManager
{
private:
	int connectedUserCount;
	map<int, ClientInfo*> clientDic;
	queue<ClientInfo*> clientQueue;
	vector<InGameManager*> ingameMgrList;
	GameManager();
	static GameManager* instance;

public:
	static GameManager* GetInstance();

	~GameManager();

	void UserLogin(int keyId, string id, string pw, string icon,ClientInfo& argClient,SOCKET mySock);	// 유저 접속
	void UserLogOut();	// 유저 로그아웃
	bool StartGameMatching(ClientInfo* client);	// 게임 매칭
	int GetConnectedUserCount() const;
	bool IsContainedKey(int key);


	int InitBoard(int keyId);
	void ReleaseBoard(int keyId);

	/*void SetInGameID(int id);
	int GetInGameID() const;*/

	int GetNowTurn(int keyId) const;
	bool GetIsInit(int keyId) const;
	bool GetIsGameOver(int keyId) const;

	//int CreateInGameId(int keyId);
	string GetOpponentId(int myKey);

	bool GameProcess(int key);

};