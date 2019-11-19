#pragma once
#include <queue>
#include <vector>
#include <map>
#include"ClientInfo.h"
#include"InGameManager.h"

using namespace std;

class InGameManager;

/// ��ü���� ���� ���� Ŭ����
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

	void UserLogin(int keyId, string id, string pw, string icon,ClientInfo& argClient,SOCKET mySock);	// ���� ����
	void UserLogOut();	// ���� �α׾ƿ�
	bool StartGameMatching(ClientInfo* client);	// ���� ��Ī
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