#ifndef FISH_H
#define FISH_H
#include "../tile.h"
// Astrae un pesce
class Fish : public Tile
{
public:
  Fish(int kindness);
  virtual ~Fish();
  // Colore della casella
  void setColor(const Cairo::RefPtr<Cairo::Context> &cr) override;
  // Vita del pesce
  double life_bar = 1.0;
  // Se il pesce si è mosso
  int moved = 0;
  // Se il pesce è vivo o morto
  bool died = false;
  // Grado di altruismo
  int kindness;
protected:
};

#endif