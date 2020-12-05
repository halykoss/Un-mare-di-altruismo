#include "area/area.h"
#include "initializer/initializer.h"
#include "tile/tile.h"
#include "utils/utils.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <sstream>
#include <mutex>
using namespace std;

int CURR_FISH = 0;
int CURR_FOOD = 0;
// Extern

// Per comunicazione con script python
int p[2], pid;

// Per Thread di calcolo
bool stop = false;

void f(Initializer *init, Area *c, mutex *mx);
int pipe_py();

int main(int argc, char **argv)
{
   Utils::get_settings_from_json();
   auto app = Gtk::Application::create(argc, argv, "it.unibo.fsc");
   mutex mtx;
   Gtk::Window win;

   win.set_title("Simulator");
   win.set_size_request(WINDOW_SIZE_W, WINDOW_SIZE_H);

   // Assegno NULL alle caselle della mappa
   Tile *map[MAP_SIZE_W][MAP_SIZE_H] = {nullptr};
   Initializer init(&map);

   // Lancio lo script in python per le statistiche
   int fd = pipe_py();

   // Rendering della mappa
   Area c(fd, &map, &mtx, &init);

   win.add(c);
   c.show();

   // Lancio il thread per i calcoli
   // thread th1(f, &init, &c, &mtx);
   int res = app->run(win);

   // Quando la finestra si chiude fermo anche il thread dei calcoli
   stop = true;
   //th1.join();
   kill(pid, SIGALRM);
   close(p[1]);
   wait(NULL);
   return res;
}

// Funzione che avvia la comunicazione con Python
int pipe_py()
{
   string cmd = "python3";
   if (pipe(p) < 0)
      exit(1);

   pid = fork();
   // caso di errore
   if (pid < 0)
   {
      cerr << "Impossibile lanciare processo" << endl;
      exit(1);
   }
   // Figlio
   if (pid == 0)
   {
      std::ostringstream oss;
      oss << REFRESH_TIME;
      close(p[1]);
      dup2(p[0], 0);
      execlp(cmd.c_str(), cmd.c_str(), "stats/stats.py", oss.str().c_str(), (char *)0);
   }
   // Padre
   return p[1];
}