#include "initializer.h"
#include "../tile/tile.h"
#include "../tile/food/food.h"
#include "../tile/fish/fish.h"

Initializer::Initializer(Tile* (*mapInit)[45][45])
{
    this->map = mapInit;

    for (int i = 0; i < 40; i++)
    {
        int r1 = rand() % 45, r2 = rand() % 45;
        if ((*map)[r1][r2] == nullptr)
        {
            (*map)[r1][r2] = new Fish{(int)((float)100 * rand() / RAND_MAX)};
        }
        else
        {
            i--;
        }
    }

	for (int i = 0; i < 5; i++)
	{
		int r1 = rand() % 45, r2 = rand() % 45;
		if ((*map)[r1][r2] == nullptr)
		{
			(*map)[r1][r2] = new Food;
		}
		else
		{
			i--;
		}
	}
}

void Initializer::updateMap(){
     int r1 = rand() % 45, r2 = rand() % 45;
        if ((*map)[r1][r2] == nullptr)
        {
            (*map)[r1][r2] = new Fish{(int)((float)100 * rand() / RAND_MAX)};
        }
}

Initializer::~Initializer()
{
}