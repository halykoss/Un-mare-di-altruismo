#include <ctime>
#include <cmath>
#include <cairomm/context.h>
#include <glibmm/main.h>
#include <cstdlib>
#include <iostream>
#include "area.h"
#include "../tile/fish/fish.h"
#include "../utils.h"
#include <mutex>

using namespace std;

Area::Area(Tile *(*mapInit)[MAP_SIZE_W][MAP_SIZE_H], mutex *mtx)
{
	this->mtx = mtx;
	map = mapInit;
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &Area::trigger_redraw), REFRESH_TIME);
}

Area::~Area()
{
}

bool Area::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{

	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();
	srand((unsigned)time(NULL));
	this->mtx->lock();
	for (int i = 0; i < MAP_SIZE_W; i++)
	{
		for (int j = 0; j < MAP_SIZE_H; j++)
		{
			cr->set_source_rgb(0.6, 0.8, 1.0);
			cr->rectangle(j * SIZE_CELL_W, i * SIZE_CELL_H, SIZE_CELL_W, SIZE_CELL_H);
			cr->fill();
		}
	}
	for (int i = 0; i < MAP_SIZE_W; i++)
	{
		for (int j = 0; j < MAP_SIZE_H; j++)
		{
			if ((*map)[i][j] != 0)
			{
				if (Fish *v = dynamic_cast<Fish *>((*map)[i][j]))
				{
					cr->save();
					cr->arc(j * SIZE_CELL_W + (SIZE_CELL_W / 2), i * SIZE_CELL_H + (SIZE_CELL_H / 2), ((float) SIZE_CELL_H + (SIZE_CELL_H / 2)), 0.0, 2.0 * M_PI); // full circle
					cr->set_source_rgba(0.0, 0.0, 0.8, 0.3);					   // partially translucent
					cr->fill_preserve();
					cr->restore(); // back to opaque black
					cr->stroke();
				}
			}
		}
	}
	for (int i = 0; i < 45; i++)
	{
		for (int j = 0; j < 45; j++)
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

bool Area::trigger_redraw()
{
	// force our program to redraw the entire clock.
	auto win = get_window();
	if (win)
	{
		Gdk::Rectangle r(0, 0, get_allocation().get_width(),
						 get_allocation().get_height());
		win->invalidate_rect(r, false);
	}
	return true;
}