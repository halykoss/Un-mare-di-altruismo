#ifndef INITILIAZER_H
#define INITILIAZER_H

#include <cstdlib>
#include <iostream>
#include "../tile/tile.h"
#include <list>
#include <chrono>
#include <mutex>
#include <random>
#include <ctime>
#include "../tile/fish/fish.h"
#include "../utils.h"

using namespace std;
// Rappresenta le iterazioni sulla mappa
class Initializer
{
public:
    Initializer(Tile *(*map)[MAP_SIZE_W][MAP_SIZE_H]);
    virtual bool updateMap(mutex *mx);
    virtual ~Initializer();
    int CURR_FISH;
    int CURR_FOOD;

private:
    // Mossa dei pesci
    void move_fish(int i, int j, int sign1, int sign2);
    void shareFoodAction(int i, int j);
    bool can_move_again();
    bool checkFood(int i, int j, int *posx, int *posy);
    Fish *procreate(Fish *v, int i, int j);
    void locate(Fish *n, int i, int j);
    std::random_device rd;
    std::mt19937 mt = std::mt19937(rd());
    std::uniform_int_distribution<> dist = std::uniform_int_distribution<>(0, 100);

protected:
    // Mappa
    Tile *(*map)[MAP_SIZE_W][MAP_SIZE_H];
    // Lista dei pesci sul campo
    list<Fish *> list_of_fish;
};

#endif