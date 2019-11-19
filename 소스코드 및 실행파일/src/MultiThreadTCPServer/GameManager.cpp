#include "GameManager.h"
#include <winsock2.h>

GameManager* GameManager::instance = nullptr;
GameManager::GameManager()
{
}

GameManager * GameManager::GetInstance()
{
	if (instance == nullptr)
		instance = new GameManager();

	return instance;
}

GameManager::~GameManager()
{
}

void GameManager::UserLogin(int keyId, string id, string pw, string icon, ClientInfo& argClient, SOCKET mySock)
{
	//todo 전적 작업 필요
	ClientInfo* client = new ClientInfo(keyId,id, pw, icon, 0,0, mySock);
	clientDic.insert(make_pair(keyId, client));
	clientQueue.push(client);
	connectedUserCount++;

	//ClientInfo* client2 = clientDic.at(keyId);
	argClient = *clientDic.at(keyId);
}

bool GameManager::StartGameMatching(ClientInfo * client)
{
	
	// vector에서 인게임 매니저 있나없나 체크
	// 있으면 자기 키값이 있나 없나 체크
	// 없으면 새로 생성
	InGameManager* ingameMgr = nullptr;
	for (int i = 0; i < ingameMgrList.size(); i++)
	{
		if (ingameMgrList[i]->IsContainKey(client->GetKeyID()))
		{
			ingameMgr = ingameMgrList[i];
			//argIngameMgr = *ingameMgr;
			return true;
		}
	}

	if (clientQueue.size() < 2)
		return false;

	if (ingameMgr == nullptr)
	{
		ingameMgr = new InGameManager();
		ingameMgrList.push_back(ingameMgr);

		ClientInfo* first = clientQueue.front();
		clientQueue.pop();
		ClientInfo* second = clientQueue.front();
		clientQueue.pop();
		ingameMgr->SetClientDic(first, second);
	}
	//argIngameMgr = *ingameMgr;
	return true;
}

int GameManager::GetConnectedUserCount() const
{
	return connectedUserCount;
}

bool GameManager::IsContainedKey(int key)
{
	int ret = false;
	if (clientDic.find(key) != clientDic.end())
		ret = true;
	return ret;
}

int GameManager::InitBoard(int keyId)
{
	InGameManager* ingameMgr = nullptr;
	for (int i = 0; i < ingameMgrList.size(); i++)
	{
		if (ingameMgrList[i]->IsContainKey(keyId))
			ingameMgr = ingameMgrList[i];
	}

	if (ingameMgr == nullptr)
	{
		cout << "ingameMgr is nullptr" << endl;
		return -1;
	}

	return ingameMgr->InitBoard();
}

string GameManager::GetOpponentId(int myKey)
{
	InGameManager* ingameMgr = nullptr;
	for (int i = 0; i < ingameMgrList.size(); i++)
	{
		if (ingameMgrList[i]->IsContainKey(myKey))
			ingameMgr = ingameMgrList[i];
	}

	if (ingameMgr == nullptr)
	{
		cout << "ingameMgr is nullptr" << endl;
		return "id error";
	}

	return ingameMgr->GetOpponentId(myKey);
}



void GameManager::ReleaseBoard(int keyId)
{
	InGameManager* ingameMgr = nullptr;
	for (int i = 0; i < ingameMgrList.size(); i++)
	{
		if (ingameMgrList[i]->IsContainKey(keyId))
			ingameMgr = ingameMgrList[i];
	}

	if (ingameMgr == nullptr)
	{
		cout << "ingameMgr is nullptr" << endl;
		return;
	}

	ingameMgr->ReleaseBoard();
}

int GameManager::GetNowTurn(int keyId) const
{
	InGameManager* ingameMgr = nullptr;
	for (int i = 0; i < ingameMgrList.size(); i++)
	{
		if (ingameMgrList[i]->IsContainKey(keyId))
			ingameMgr = ingameMgrList[i];
	}

	if (ingameMgr == nullptr)
	{
		cout << "ingameMgr is nullptr" << endl;
		return -1;
	}

	return ingameMgr->GetNowTurn();
}

bool GameManager::GetIsInit(int keyId) const
{
	InGameManager* ingameMgr = nullptr;
	for (int i = 0; i < ingameMgrList.size(); i++)
	{
		if (ingameMgrList[i]->IsContainKey(keyId))
			ingameMgr = ingameMgrList[i];
	}

	if (ingameMgr == nullptr)
	{
		cout << "ingameMgr is nullptr" << endl;
		return false;
	}

	return ingameMgr->GetIsInit();
}

bool GameManager::GetIsGameOver(int keyId) const
{
	InGameManager* ingameMgr = nullptr;
	for (int i = 0; i < ingameMgrList.size(); i++)
	{
		if (ingameMgrList[i]->IsContainKey(keyId))
			ingameMgr = ingameMgrList[i];
	}

	if (ingameMgr == nullptr)
	{
		cout << "ingameMgr is nullptr" << endl;
		return false;
	}
	return ingameMgr->GetIsGameOver();
}

bool GameManager::GameProcess(int key)
{
	InGameManager* ingameMgr = nullptr;
	for (int i = 0; i < ingameMgrList.size(); i++)
	{
		if (ingameMgrList[i]->IsContainKey(key))
			ingameMgr = ingameMgrList[i];
	}

	if (ingameMgr == nullptr)
	{
		cout << "ingameMgr is nullptr" << endl;
		return false;
	}

	return ingameMgr->GameProcess(key);
}