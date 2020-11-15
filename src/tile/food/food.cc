#include "food.h"

Food::Food(){
    this->t = Tile::type::food;
    }
Food::~Food(){}

void Food::setColor(const Cairo::RefPtr<Cairo::Context> &cr){
    // Imposto il colore
    cr->set_source_rgb(0.0, 0.8, 0.0);
}