#ifndef AREA_H
#define AREA_H

#include <gtkmm/drawingarea.h>
#include "../tile/tile.h"
#include "../initializer/initializer.h"
#include <mutex>
#include <gtkmm.h>
#include <thread>
#include <unistd.h>
#include "../utils/utils.h"
// Il disegno della mappa su cui
// i pesci si muovono
class Area : public Gtk::DrawingArea
{

public:
  Area(int fd);
  virtual ~Area();
  void trigger_redraw();

protected:
  //Override il disegno di default
  bool on_draw(const Cairo::RefPtr<Cairo::Context> &cr) override;
  Glib::Dispatcher m_Dispatcher;
  std::mutex *mtx[3][3];
  std::mutex fdMtx;
  // Descrittore del file di comunicazione
  // con Python
  int fd;
  Initializer *init[3][3];
  void f(int i, int j);
  void send_out(int i, int j, Initializer::state *s);
  void notify();
  std::thread *m_WorkerThread;
};

#endif // GTKMM_EXAMPLE_CLOCK_H