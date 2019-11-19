#include "GameManager.h"


GameManager* GameManager::instance = nullptr;
GameManager::GameManager()
{
	SetGameState(LoginState);
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

E_GameState GameManager::GetGameState() const
{
	return gameState;
}

void GameManager::SetGameState(E_GameState state)
{
	if (state == gameState)
		return;

	gameState = state;
}