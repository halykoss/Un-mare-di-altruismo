#include "src/area/area.h"
#include "src/initializer/initializer.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include "src/tile/tile.h"
#include "src/utils.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <sstream>
#include <mutex>

using namespace std;

// Extern
int CURR_FISH = 0;
int CURR_FOOD = 0;

// Per comunicazione con script python
int p[2], pid;

// Per Thread di calcolo
bool stop = false;

void f(Initializer *init, Area *c, mutex *mx);
int pipe_py();

int main(int argc, char **argv)
{
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
   Area c(fd, &map, &mtx);

   win.add(c);
   c.show();

   // Lancio il thread per i calcoli
   thread th1(f, &init, &c, &mtx);
   int res = app->run(win);

   // Quando la finestra si chiude fermo anche il thread dei calcoli
   stop = true;
   th1.join();
   close(p[1]);
   kill(pid, SIGALRM);
   wait(NULL);
   return res;
}

// Funzione che aggiorna la mappa
void f(Initializer *init, Area *c, mutex *mx)
{
   int num_of_iteration = 0;
   bool all_died = false;
   while (!all_died && !stop)
   {
      all_died = init->updateMap(mx);
      if (all_died == true)
      {
         cout << "Tutti morti";
         break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_TIME));
      num_of_iteration++;
   }
   cout << "Numero di iterazioni : " << num_of_iteration << endl;
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
      execlp(cmd.c_str(), cmd.c_str(), "stats.py", oss.str().c_str(), (char *)0);
   }
   // Padre
   return p[1];
}