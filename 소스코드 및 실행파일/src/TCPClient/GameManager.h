#pragma once

enum E_GameState
{
	LoginState,
	Lobby,
	InGame,
	Result
};

class GameManager
{
private:
	E_GameState gameState;

	GameManager();
	static GameManager* instance;

public:
	static GameManager* GetInstance();

	~GameManager();
	E_GameState GetGameState() const;
	void SetGameState(E_GameState state);
};

