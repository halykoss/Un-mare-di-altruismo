#ifndef TILE_H
#define TILE_H
#include <cairomm/context.h>

class Tile 
{
public:
  enum class type {
    fish,
    food
  };
  Tile();
  virtual ~Tile();
  void virtual setColor(const Cairo::RefPtr<Cairo::Context> &cr) {};
  type t;
};

#endif