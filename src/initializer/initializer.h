#ifndef INITILIAZER_H
#define INITILIAZER_H

#include <cstdlib>
#include <iostream>
#include "../tile/tile.h"

class Initializer 
{
public:
  Initializer(Tile* (*map)[45][45]);
  virtual void updateMap();
  virtual ~Initializer();

protected:
  Tile* (*map)[45][45];

};

#endif