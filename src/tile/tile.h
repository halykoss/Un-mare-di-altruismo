#ifndef TILE_H
#define TILE_H
#include <cairomm/context.h>

class Tile 
{
public:
  Tile();
  virtual ~Tile();
  void virtual setColor(const Cairo::RefPtr<Cairo::Context> &cr) {};
};

#endif