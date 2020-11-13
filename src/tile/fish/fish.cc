#include "fish.h"
#include "../tile.h"
Fish::Fish(int kindness){
    this->t = Tile::type::fish;
    this->kindness = kindness;
}

Fish::~Fish(){}

void Fish::setColor (const Cairo::RefPtr<Cairo::Context> &cr){
    cr->set_source_rgba(((float) this->kindness / 100), 0, 0,(float) this->life_bar);
}