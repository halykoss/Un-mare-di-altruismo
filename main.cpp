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

int CURR_FISH = 0;

void f(Initializer *init, Area *c, mutex* mx)
{
   int count = 0;
   bool all_died = false;
   while (!all_died && !stop)
   {
      all_died = init->updateMap(mx);
      if(all_died == true){
         cout << "Tutti morti";
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_TIME));
      count++;
   }
   cout << "Numero di pesci : " << CURR_FISH << endl;
   cout << "Numero di iterazioni : " << count << endl;
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
