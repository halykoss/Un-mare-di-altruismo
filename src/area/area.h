#ifndef GTKMM_EXAMPLE_CLOCK_H
#define GTKMM_EXAMPLE_CLOCK_H

#include <gtkmm/drawingarea.h>
#include "../tile/tile.h"

class Area : public Gtk::DrawingArea
{
public:
  Area(Tile* (*mapinit)[45][45]);
  virtual ~Area();
  bool trigger_redraw();

protected:
  //Override default signal handler:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  Tile* (*map)[45][45];
  double m_radius;
  double m_line_width;

};

#endif // GTKMM_EXAMPLE_CLOCK_H