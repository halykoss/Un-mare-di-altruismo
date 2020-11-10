#include <ctime>
#include <cmath>
#include <cairomm/context.h>
#include <glibmm/main.h>
#include <cstdlib>
#include <iostream>
#include "area.h"

using namespace std;

Area::Area(Tile* (*mapInit)[45][45])
	: m_radius(0.42), m_line_width(0.05)
{
	map = mapInit;
	//Glib::signal_timeout().connect( sigc::mem_fun(*this, &Area::on_timeout), 1000 );
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
	// coordinates for the center of the window
	int xc, yc;
	xc = width / 2;
	yc = height / 2;

	for (int i = 0; i < 45; i++)
	{
		for (int j = 0; j < 45; j++)
		{
			cr->set_source_rgb(0.6, 0.8, 1.0);
			cr->rectangle(j * 20, i * 20, 20, 20);
			cr->fill();
		}
	}
	for (int i = 0; i < 45; i++)
	{
		for (int j = 0; j < 45; j++)
		{
			if ((*map)[i][j] != 0)
			{
  				cr->save();
  				cr->arc(j * 20 + 10, i * 20 + 10, 100 / 4.0, 0.0, 2.0 * M_PI); // full circle
  				cr->set_source_rgba(0.0, 0.0, 0.8, 0.6);    // partially translucent
  				cr->fill_preserve();
  				cr->restore();  // back to opaque black
  				cr->stroke();
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
				cr->rectangle(j * 20, i * 20, 20, 20);
				cr->fill();
			}
		}
	} 
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