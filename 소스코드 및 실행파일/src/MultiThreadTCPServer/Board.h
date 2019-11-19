#ifndef BOARD_H
#define BOARD_H

#include "Player.h"
#include <vector>
#include <map>

class Board
{

public:
	Board();
	Board(std::map<int, int> &pSnakes, std::map<int, int> &pLadders);
	void AddPlayer(Player* pPlayer);
	Player *NextPlayer();
	Player *CurrentPlayer();
	int UpdatedPosition(int pPosition);
	void DisplayStatus();
private :
	std::map<int, int> mSnakes;
	std::map<int, int> mLadders;
	std::vector<Player*> mPlayer;
	int mCurrentPlayer;
};

#endif