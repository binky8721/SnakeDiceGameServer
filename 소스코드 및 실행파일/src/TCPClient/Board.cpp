
#include "Board.h"
#include <map>

Board::Board()
	: mCurrentPlayer(0)
{
	mSnakes[17] = 7;
	mSnakes[54] = 34;
	mSnakes[62] = 18;
	mSnakes[64] = 60;
	mSnakes[87] = 24;
	mSnakes[93] = 73;
	mSnakes[95] = 75;
	mSnakes[99] = 78;

	mLadders[4] = 14;
	mLadders[9] = 31;
	mLadders[20] = 38;
	mLadders[28] = 84;
	mLadders[40] = 59;
	mLadders[63] = 81;
	mLadders[71] = 91;
}

Board::Board(std::map<int, int> &pSnakes, std::map<int, int> &pLadders) : mCurrentPlayer(0)
{
	mSnakes.insert(pSnakes.begin(), pSnakes.end());
	mLadders.insert(pLadders.begin(), pLadders.end());
}
	
void Board::AddPlayer(Player* pPlayer)//클라, 서버 둘다
{
	mPlayer.push_back(pPlayer);
}

Player* Board::NextPlayer()//서버만
{
	if(mPlayer.size() == 0)
		return nullptr;

	mCurrentPlayer++;
	mCurrentPlayer = mCurrentPlayer % mPlayer.size();
	return mPlayer[mCurrentPlayer];
}
	
int Board::UpdatedPosition(int pPosition)//클라,서버 둘다 있어야
{
	std::map<int, int>::iterator it = mSnakes.find(pPosition);
	if(it != mSnakes.end())
		return mSnakes[pPosition];

	it = mLadders.find(pPosition);
	if(it!=mLadders.end())
		return mLadders[pPosition];

	return pPosition;
}

void Board::DisplayStatus()
{
	for(auto it = mPlayer.begin(); it != mPlayer.end(); it++)
		(*it)->Display();
}
