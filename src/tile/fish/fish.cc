#include "fish.h"
#include "../tile.h"
#include "math.h"

// ho fatto in modo che l'energia non scenda sotto zero, quando diventa zero il pesce deve morire 
// IDEA: rimuovere a fine turno i pesci con energia zero


Fish::Fish(int kindness, double mass, int speed){
    this->t = Tile::type::fish;
    this->kindness = kindness;
    this->mass = mass;
    this->speed = speed;
}

Fish::~Fish(){}

void Fish::setColor (const Cairo::RefPtr<Cairo::Context> &cr){
    // Imposto il colore
    cr->set_source_rgba(((float) this->kindness / 100), 0, 0,(float) this->life_bar);
}

void Fish::shareFood (Fish *f1) {    // ATTENZIONE NON CANCELLA IL CIBO DALLA MAPPA (ma è da fare)
	double enFood = 0.33; //provvisorio
	double enGain = enFood/2*(f1->life_bar/(f1->life_bar + this->life_bar) + f1->kindness/(f1->kindness + this->kindness));
	this->life_bar += enGain;
	f1->life_bar += (enFood - enGain);
	if (this->life_bar>1) {			//CONTROLLA CHE L'ENERGIA DEL PESCE NON SUPERI 1
		this->life_bar = 1;
	}
	if (f1->life_bar>1) {
		f1->life_bar = 1;
	}
}

void Fish::fightFood (Fish *f1) {
	double prestanza1;
	double prestanza2;
	double enLoss;      //energia consumata per combattere (uguale per entrambi)
	prestanza1 = (f1->mass)*(f1->speed) + 2*(f1->life_bar)/(0.01 + abs((f1->mass)*(f1->speed)-(this->mass)*(this->speed)));
	prestanza2 = (this->mass)*(this->speed) + 2*(this->life_bar)/(0.01 + abs((f1->mass)*(f1->speed)-(this->mass)*(this->speed)));
	
	if (abs((f1->mass)*(f1->speed)-(this->mass)*(this->speed))==0) {  //evitiamo infinito
		enLoss = 0.4;
	}
	else {
		enLoss = 0.02/abs((f1->mass)*(f1->speed)-(this->mass)*(this->speed));
		if (enLoss>0.4) {						// la massima energia persa è 0.4
			enLoss = 0.4;
		}
	}
	
	// controlliamo se sono morti per combattere prima di mangiare
	if (prestanza1==prestanza2) {    //caso altamente improbabile ma per sicurezza lo metto
		f1->life_bar = 0;
		this->life_bar= 0;
	}
	else {
		f1->life_bar -= enLoss;
		if (f1->life_bar<0) {f1->life_bar=0;}
		this->life_bar -= enLoss;
		if (this->life_bar<0) {this->life_bar=0;}
	}
	
	
	// faccio mangiare chi vince se è ancora vivo, altrimenti l'altro se è ancora vivo, altrimenti nessuno (il cibo dovrebbe rimanere sulla mappa)
	
	if (prestanza1>prestanza2) {
		if (f1->life_bar>0) {
			f1->life_bar += 0.33; //provvisorio
		}
		else if (this->life_bar>0) {
			this->life_bar += 0.33;
		}	
	}
	
	if (prestanza1<prestanza2) {
			if (this->life_bar>0) {
			this->life_bar += 0.33; 
		}
		else if (f1->life_bar>0) {
			f1->life_bar += 0.33;
		}
	}
	
}

void Fish::movementEnergy (int steps) {
	double movementCost = 0.003*(this->mass)*steps*steps; //da testare il coefficiente
	this->life_bar -= movementCost;
	if (this->life_bar<0) {
		this->life_bar = 0;
	}
}


