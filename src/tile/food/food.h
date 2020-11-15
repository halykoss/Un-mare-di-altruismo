#ifndef FOOD_H
#define FOOD_H
#include "../tile.h"
// Astrae il cibo
class Food : public Tile
{
public:
  Food();
  virtual ~Food();
  // Imposta il colore della casella
  void setColor(const Cairo::RefPtr<Cairo::Context> &cr) override;
};

#endif