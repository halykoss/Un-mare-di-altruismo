#ifndef TILE_H
#define TILE_H
#include <cairomm/context.h>

class Tile 
{
public:
  // Tipi di caselle
  enum class type {
    fish,
    food,
    tile
  };
  type t;
  Tile();
  virtual ~Tile();
  // Funzione che colora le caselle
  void virtual setColor(const Cairo::RefPtr<Cairo::Context> &cr) {};
};

#endif