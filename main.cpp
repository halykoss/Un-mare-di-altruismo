#include "src/area/area.h"
#include "src/initializer/initializer.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include "src/tile/tile.h"
#include "src/utils.h"
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

bool stop = false;

void f(Initializer *init, Area *c, mutex* mx)
{
   while (!stop)
   {
      mx->lock();
      init->updateMap();
      mx->unlock();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
   }
}

int main(int argc, char **argv)
{
   auto app = Gtk::Application::create(argc, argv, "it.unibo.fsc");
   mutex mtx;
   Gtk::Window win;
   win.set_title("Simulator");
   win.set_size_request(WINDOW_SIZE_W, WINDOW_SIZE_H);
   Tile *map[MAP_SIZE_W][MAP_SIZE_H] = {nullptr};
   Initializer init(&map);
   Area c(&map,&mtx);

   win.add(c);
   c.show();

   thread th1(f, &init, &c,&mtx);
   int res = app->run(win);
   stop = true;
   th1.join();
   return res;
}
