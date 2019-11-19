#pragma once
#include <string>

class Player
{
public:
	enum Status
	{
		None,
		Win
	};
	Player(std::string pName);
	int ThroughDice();
 	Status Move(int pPosition);

	std::string GetName();
	int GetPosition();
	void Display();
private:
	std::string mName;
	int mPosition;
};
