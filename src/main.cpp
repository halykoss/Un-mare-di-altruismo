#include "area/area.h"
#include "field/field.h"
#include "utils/utils.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <thread>
#include <mutex>
using namespace std;
// Extern

// Per comunicazione con script python
int p[2], pid;

void f(Initializer *init, Area *c, mutex *mx);
int pipe_py();

int main(int argc, char **argv)
{
   Utils::get_settings_from_json();
   auto app = Gtk::Application::create(argc, argv, "it.unibo.fsc");
   Gtk::Window win;

   win.set_title("Simulator");
   //win.set_size_request(Utils::WINDOW_SIZE_W, Utils::WINDOW_SIZE_H);

   // Lancio lo script in python per le statistiche
   int fd = pipe_py();

   // Rendering della mappa
   Area c(fd);

   win.add(c);
   c.show();
   win.set_resizable(false);
   win.set_size_request(810,810);
   // Lancio il thread per i calcoli
   int res = app->run(win);
   c.stop = true;
   for(int i = 0; i < 9; i++){
   	c.m_WorkerThread[i]->join();
   }
   kill(pid, SIGALRM);
   kill(pid, SIGKILL);
   close(p[1]);
   wait(nullptr);
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
