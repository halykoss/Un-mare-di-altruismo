#ifndef FOOD_H
#define FOOD_H
#include "../tile.h"
class Food : public Tile
{
public:
  Food();
  virtual ~Food();
  void setColor(const Cairo::RefPtr<Cairo::Context> &cr) override;
};

#endif