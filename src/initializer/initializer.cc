#include "initializer.h"
#include "../tile/tile.h"
#include "../tile/food/food.h"
#include "../tile/fish/fish.h"
#include "../utils.h"
#include <iostream>
#include <chrono>
#include <ctime>

using namespace std;

Initializer::Initializer(Tile *(*mapInit)[MAP_SIZE_W][MAP_SIZE_H])
{
    this->map = mapInit;
    start = std::chrono::system_clock::now();
    for (int i = 0; i < NUN_OF_FISH; i++)
    {
        int r1 = rand() % MAP_SIZE_W, r2 = rand() % MAP_SIZE_H;
        if ((*map)[r1][r2] == nullptr)
        {
            (*map)[r1][r2] = new Fish{(int)((float)100 * rand() / RAND_MAX)};
        }
        else
        {
            i--;
        }
    }

    for (int i = 0; i < NUM_OF_FOOD; i++)
    {
        int r1 = rand() % MAP_SIZE_W, r2 = rand() % MAP_SIZE_H;
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

void Initializer::updateMap()
{
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - this->start;

    if (elapsed_seconds.count() > SPAWN_TIME)
    {
        int r1 = rand() % MAP_SIZE_W, r2 = rand() % MAP_SIZE_H;
        if ((*map)[r1][r2] == nullptr)
        {
            (*map)[r1][r2] = new Food;
        }
        this->start = end;
    } 

    for (int i = 0; i < MAP_SIZE_W; i++)
    {
        for (int j = 0; j < MAP_SIZE_H; j++)
        {
            if (Fish *v = dynamic_cast<Fish *>((*map)[i][j]))
            {
                int r1 = (rand() % 2) - (rand() % 2);
                int r2 = (rand() % 2) - (rand() % 2);
                if (i + r1 < MAP_SIZE_W && i + r1 >= 0 && j + r2 < MAP_SIZE_H && i + r2 >= 0 && ((*map)[i + r1][j + r2] == 0))
                {
                    (*map)[i + r1][j + r2] = (*map)[i][j];
                    (*map)[i][j] = nullptr;
                }
            }
        }
    }
}

Initializer::~Initializer()
{
}