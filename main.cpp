#include "src/area/area.h"
#include "src/initializer/initializer.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include "src/tile/tile.h"
#include <iostream> 
#include <thread> 

using namespace std; 

bool stop = false;

void f(Initializer* init,Area* c){
      while(!stop){
         init->updateMap();
         c->trigger_redraw();
          std::this_thread::sleep_for (std::chrono::seconds(1));
      }  
}

int main(int argc, char **argv)
{
   auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

   Gtk::Window win;
   win.set_title("Simulator");
   win.set_size_request(900, 900);
   Tile* map[45][45] = {nullptr};
   Initializer init(&map);
   Area c(&map);

   win.add(c);
   c.show();

   thread th1(f, &init, &c);
   int res = app->run(win);
   stop = true;
   th1.join(); 
   return res;
}
