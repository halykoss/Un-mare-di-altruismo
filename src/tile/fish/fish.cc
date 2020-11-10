#include "fish.h"

Fish::Fish(int kindness){
    this->kindness = kindness;
}

Fish::~Fish(){}

void Fish::setColor (const Cairo::RefPtr<Cairo::Context> &cr){
    cr->set_source_rgb(((float) this->kindness / 100), 0, 0);
}