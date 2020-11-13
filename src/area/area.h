#ifndef GTKMM_EXAMPLE_CLOCK_H
#define GTKMM_EXAMPLE_CLOCK_H

#include <gtkmm/drawingarea.h>
#include "../tile/tile.h"
#include <mutex>
#include "../utils.h"

class Area : public Gtk::DrawingArea
{
public:
  Area(Tile* (*mapinit)[MAP_SIZE_W][MAP_SIZE_H], std::mutex* mtx);
  virtual ~Area();
  bool trigger_redraw();

protected:
  //Override default signal handler:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  Tile* (*map)[MAP_SIZE_W][MAP_SIZE_H];
  std::mutex* mtx;

};

#endif // GTKMM_EXAMPLE_CLOCK_H