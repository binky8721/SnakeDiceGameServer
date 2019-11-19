#include "Dice.h"
#include <cstdlib>
#include <ctime>

int Dice::Roll()
{
	srand((unsigned int)time(0));
	return (rand() % 6) + 1;
}