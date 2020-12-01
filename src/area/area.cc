#include <ctime>
#include <cmath>
#include <cairomm/context.h>
#include <glibmm/main.h>
#include <cstdlib>
#include <iostream>
#include "area.h"
#include "../tile/fish/fish.h"
#include "../initializer/initializer.h"
#include "../utils.h"
#include <mutex>
#include <thread>
#include <unistd.h>

using namespace std;

Area::Area(int fd, Tile *(*mapInit)[MAP_SIZE_W][MAP_SIZE_H], mutex *mtx, Initializer *init) : m_Dispatcher()
{
	this->fd = fd;
	this->mtx = mtx;
	this->init = init;
	map = mapInit;
	m_Dispatcher.connect(sigc::mem_fun(*this, &Area::trigger_redraw));
	m_WorkerThread = new std::thread(sigc::mem_fun(*this, &Area::f));
	send_out();
}

Area::~Area()
{
}

bool Area::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
	// Ottengo le informazioni sulla finestra
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();
	srand((unsigned)time(NULL));
	this->mtx->lock();
	// Stampo il campo
	for (int i = 0; i < MAP_SIZE_W; i++)
	{
		for (int j = 0; j < MAP_SIZE_H; j++)
		{
			cr->set_source_rgb(0.6, 0.8, 1.0);
			cr->rectangle(j * SIZE_CELL_W, i * SIZE_CELL_H, SIZE_CELL_W, SIZE_CELL_H);
			cr->fill();
		}
	}
	// Mostro il raggio visivo dei pesci
	for (int i = 0; i < MAP_SIZE_W; i++)
	{
		for (int j = 0; j < MAP_SIZE_H; j++)
		{
			if ((*map)[i][j] != 0)
			{
				if (Fish *v = dynamic_cast<Fish *>((*map)[i][j]))
				{
					cr->save();
					cr->arc(j * SIZE_CELL_W + (SIZE_CELL_W / 2), i * SIZE_CELL_H + (SIZE_CELL_H / 2), ((float)SENSOR_RADIUS * SIZE_CELL_H + (SIZE_CELL_H / 2)), 0.0, 2.0 * M_PI); // circonferenza intera
					cr->set_source_rgba(0.0, 0.0, 0.8, 0.3);																													  // trasparenza
					cr->fill_preserve();
					cr->restore(); // Ritorno al colore opaco
					cr->stroke();
				}
			}
		}
	}
	// Stampo la cella con cibo/pesce
	for (int i = 0; i < MAP_SIZE_W; i++)
	{
		for (int j = 0; j < MAP_SIZE_H; j++)
		{
			if ((*map)[i][j] != 0)
			{
				(*map)[i][j]->setColor(cr);
				cr->rectangle(j * SIZE_CELL_W, i * SIZE_CELL_H, SIZE_CELL_W, SIZE_CELL_H);
				cr->fill();
			}
		}
	}
	this->mtx->unlock();
	return true;
}

void Area::trigger_redraw()
{
	// forza il programma a disegnare di nuovo la mappa
	auto win = get_window();
	if (win)
	{
		Gdk::Rectangle r(0, 0, get_allocation().get_width(),
						 get_allocation().get_height());
		win->invalidate_rect(r, false);
	}
}

void Area::notify()
{
	m_Dispatcher.emit();
}
// Funzione che aggiorna la mappa
void Area::f()
{
	int num_of_iteration = 0;
	bool all_died = false;
	while (!all_died)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_TIME));
		all_died = init->updateMap(this->mtx);
		send_out();
		// Se sono tutti morti
		if (all_died == true)
		{
			cout << "Tutti morti";
			break;
		}
		num_of_iteration++;
		this->notify();
	}
	close(this->fd);
	cout << "Numero di iterazioni : " << num_of_iteration << endl;
}

void Area::send_out()
{
	// Scrivo al software in Python il numero di pesci e di cibo
	this->mtx->lock();
	write(this->fd, &init->CURR_FISH, sizeof(init->CURR_FISH));
	write(this->fd, &init->CURR_FOOD, sizeof(init->CURR_FOOD));
	for (int i = 0; i < MAP_SIZE_W; i++)
	{
		for (int j = 0; j < MAP_SIZE_H; j++)
		{
			if ((*map)[i][j] != 0)
			{
				if (Fish *v = dynamic_cast<Fish *>((*map)[i][j]))
				{
					write(this->fd, &(v->kindness), sizeof(v->kindness));
				}
			}
		}
	}
	this->mtx->unlock();
}