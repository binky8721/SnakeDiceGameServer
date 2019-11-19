
#include "player.h"
#include "Dice.h"

#include <iostream>

Player::Player(std::string pName) 
	: mName(pName), mPosition(0)
{
}

int Player::ThroughDice()
{
	Dice d;
	return d.Roll();
}
	
Player::Status Player::Move(int pPosition)
{
	Player::Status status = Player::Status::None;
	mPosition = pPosition;
	if(mPosition >= 100)
		status = Player::Status::Win;
	return status;
}

std::string Player::GetName()
{
	return mName;
}

int Player::GetPosition()
{
	return mPosition;
}

void Player::Display()
{
	std::cout<<"name : " << mName <<" position : " << mPosition << std::endl;
}
