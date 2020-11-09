#include "src/area.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>

int main(int argc, char** argv)
{
   auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

   Gtk::Window win;
   win.set_title("Cairomm Clock");
   win.set_size_request(900,900);
   Area c;
   win.add(c);
   c.show();

   return app->run(win);
}
