#ifndef AREA_H
#define AREA_H

#include <gtkmm/drawingarea.h>
#include "../tile/tile.h"
#include <mutex>
#include "../utils.h"
// Il disegno della mappa su cui
// i pesci si muovono
class Area : public Gtk::DrawingArea
{
    
public:
  Area(int fd,Tile* (*mapinit)[MAP_SIZE_W][MAP_SIZE_H], std::mutex* mtx);
  virtual ~Area();
  bool trigger_redraw();

protected:
  //Override il disegno di default
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  // Mappa
  Tile* (*map)[MAP_SIZE_W][MAP_SIZE_H];
  std::mutex* mtx;
  // Descrittore del file di comunicazione
  // con Python
  int fd;
};

#endif // GTKMM_EXAMPLE_CLOCK_H