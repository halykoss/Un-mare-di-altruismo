#ifndef INITILIAZER_H
#define INITILIAZER_H

#include <cstdlib>
#include <iostream>
#include "../tile/tile.h"
#include <chrono>
#include <ctime>

class Initializer 
{
public:
  Initializer(Tile* (*map)[MAP_SIZE_W][MAP_SIZE_H]);
  virtual void updateMap();
  virtual ~Initializer();

protected:
  Tile* (*map)[MAP_SIZE_W][MAP_SIZE_H];
  std::chrono::_V2::system_clock::time_point start;

};

#endif