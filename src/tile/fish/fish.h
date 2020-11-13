#ifndef FISH_H
#define FISH_H
#include "../tile.h"
class Fish : public Tile
{
public:
  Fish(int kindness);
  virtual ~Fish();
  void setColor(const Cairo::RefPtr<Cairo::Context> &cr) override;
  double life_bar = 1.0;
  int moved = 0;
  bool died = false;
protected:
  int kindness;
};

#endif