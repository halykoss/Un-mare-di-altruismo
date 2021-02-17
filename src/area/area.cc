#include <ctime>
#include <cmath>
#include <cairomm/context.h>
#include <cstdlib>
#include <iostream>
#include "area.h"
#include <mutex>
#include <thread>
#include <boost/bind.hpp>
#include <unistd.h>

using namespace std;

Area::Area(int fd) : m_Dispatcher()
{
	this->fd = fd;
	for (int k = 0; k < 3; k++)
	{
		for (int ky = 0; ky < 3; ky++)
		{
			this->mtx[k][ky] = new std::mutex();
			this->init[k][ky] = new Initializer();
		}
	}
	m_Dispatcher.connect(sigc::mem_fun(*this, &Area::trigger_redraw));
	for (int k = 0; k < 3; k++)
	{
		for (int ky = 0; ky < 3; ky++)
		{
			Initializer::state *s = this->init[k][ky]->getStringState(k, ky);
			m_WorkerThread[k * 3 + ky] = new std::thread(boost::bind(&Area::f, this, k, ky));
			send_out(k, ky, s);
		}
	}
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
	Utils::WINDOW_SIZE_W = width / 3;
	Utils::WINDOW_SIZE_H = height / 3;
	Utils::SIZE_CELL_W = Utils::WINDOW_SIZE_W / MAP_SIZE_W;
	Utils::SIZE_CELL_H = Utils::WINDOW_SIZE_H / MAP_SIZE_H;
	// Stampo i campi
	for (int k = 0; k < 3; k++)
	{
		for (int ky = 0; ky < 3; ky++)
		{
			this->mtx[k][ky]->lock();
			auto map = init[k][ky]->map;
			// Stampo il fondo blu del campo
			for (int i = 0; i < MAP_SIZE_W; i++)
			{
				for (int j = 0; j < MAP_SIZE_H; j++)
				{
					cr->set_source_rgb(0.6, 0.8, 1.0);
					cr->rectangle((k * Utils::WINDOW_SIZE_W) + j * Utils::SIZE_CELL_W, (ky * Utils::WINDOW_SIZE_H) + i * Utils::SIZE_CELL_H, Utils::SIZE_CELL_W, Utils::SIZE_CELL_H);
					cr->fill();
				}
			}
			// Mostro il raggio visivo dei pesci
			for (int i = 0; i < MAP_SIZE_W; i++)
			{
				for (int j = 0; j < MAP_SIZE_H; j++)
				{
					if (map[i][j] != nullptr)
					{
						if (map[i][j]->t == Tile::type::fish)
						{
							cr->save();
							cr->arc((k * Utils::WINDOW_SIZE_W) + j * Utils::SIZE_CELL_W + (Utils::SIZE_CELL_W / 2), (ky * Utils::WINDOW_SIZE_H) + i * Utils::SIZE_CELL_H + (Utils::SIZE_CELL_H / 2), ((float)SENSOR_RADIUS * Utils::SIZE_CELL_H + (Utils::SIZE_CELL_H / 2)), 0.0, 2.0 * M_PI); // circonferenza intera
							cr->set_source_rgba(0.0, 0.0, 0.8, 0.3);																																																						   // trasparenza
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
					if (map[i][j] != nullptr)
					{
						map[i][j]->setColor(cr);
						cr->rectangle((k * Utils::WINDOW_SIZE_W) + j * Utils::SIZE_CELL_W, (ky * Utils::WINDOW_SIZE_H) + i * Utils::SIZE_CELL_H, Utils::SIZE_CELL_W, Utils::SIZE_CELL_H);
						cr->fill();
					}
				}
			}
			this->mtx[k][ky]->unlock();
		}
	}
	cr->set_line_width(4.0);
	cr->set_source_rgb(0.0, 0.0, 0.0);
	cr->move_to(Utils::WINDOW_SIZE_W, 0);
  	cr->line_to(Utils::WINDOW_SIZE_W, Utils::WINDOW_SIZE_H * 3);
	cr->move_to(Utils::WINDOW_SIZE_W * 2, 0);
  	cr->line_to(Utils::WINDOW_SIZE_W * 2, Utils::WINDOW_SIZE_H * 3);
	cr->move_to(0, Utils::WINDOW_SIZE_H);
  	cr->line_to(Utils::WINDOW_SIZE_W * 3, Utils::WINDOW_SIZE_H);
	cr->move_to(0, Utils::WINDOW_SIZE_H * 2);
  	cr->line_to(Utils::WINDOW_SIZE_W * 3, Utils::WINDOW_SIZE_H * 2);
  	cr->stroke();
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
void Area::f(int i, int j)
{
	int num_of_iteration = 0;
	bool all_died = false;
	while (!all_died && !stop.load())
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_TIME));
		this->mtx[i][j]->lock();
		all_died = init[i][j]->updateMap();
		init[i][j]->epoch++;
		Initializer::state *s = init[i][j]->getStringState(i, j);
		this->mtx[i][j]->unlock();
		//new std::thread(boost::bind(&Area::send_out, this, i, j, s));
		this->send_out(i, j, s);
		// Se sono tutti morti
		if (all_died == true)
		{
			this->notify();
			break;
		}
		num_of_iteration++;
		this->notify();
	}
	//close(this->fd);
	//cout << "Numero di iterazioni : " << num_of_iteration << endl;
}

void Area::send_out(int i, int j, Initializer::state *s)
{
	// Scrivo al software in Python il numero di pesci e di cibo
	fdMtx.lock();
	cout << s->toString;
	int buffSize = snprintf(NULL, 0, "%d;%d;%d;%d;%d\n", i, j, s->NUM_OF_FISH, s->CURR_FOOD, s->epoch);
	char c[buffSize];
	sprintf(c, "%d;%d;%d;%d;%d\n", i, j, s->NUM_OF_FISH, s->CURR_FOOD, s->epoch);
	write(this->fd, c, sizeof(c));
	for (int i = 0; i < s->NUM_OF_FISH; i++)
	{
		int dim = snprintf(NULL, 0, "%d;%d;%f;%d;%lf\n", s->kindness[i], s->speed[i], s->triggerEnergy[i], s->currLife[i], s->life_bar[i]);
		char buf[dim];
		sprintf(buf, "%d;%d;%f;%d;%lf\n", s->kindness[i], s->speed[i], s->triggerEnergy[i], s->currLife[i], s->life_bar[i]);
		write(this->fd, buf, sizeof(buf));
	}
	fdMtx.unlock();
	delete s;
}
