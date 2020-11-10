#ifndef FISh_H
#define FISH_H
#include "../tile.h"
class Fish : public Tile
{
public:
  Fish(int kindness);
  virtual ~Fish();
  void setColor(const Cairo::RefPtr<Cairo::Context> &cr) override;
protected:
  int kindness;
};

#endif