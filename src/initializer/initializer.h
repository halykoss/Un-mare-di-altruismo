#ifndef INITILIAZER_H
#define INITILIAZER_H

#include <cstdlib>
#include <iostream>
#include "../tile/tile.h"
#include <list>
#include <chrono>
#include <mutex>
#include <ctime>
#include "../tile/fish/fish.h"
#include "../utils.h"

using namespace std;
class Initializer 
{
public:
  Initializer(Tile* (*map)[MAP_SIZE_W][MAP_SIZE_H]);
  virtual bool updateMap(mutex* mx);
  virtual ~Initializer();

private:
  void move_fish(int i, int j,int sign1, int sign2);
protected:
  Tile* (*map)[MAP_SIZE_W][MAP_SIZE_H];
  std::chrono::_V2::system_clock::time_point start;
  list <Fish *> list_of_fish;
};

#endif