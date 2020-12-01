#include "fish.h"
#include "../tile.h"
#include "math.h"
#include <iostream>
// ho fatto in modo che l'energia non scenda sotto zero, quando diventa zero il pesce deve morire 

using namespace std;

Fish::Fish(int kindness, int speed, float triggerEnergy){
    this->t = Tile::type::fish;
    this->kindness = kindness;
    this->speed = speed;
    this->life_bar = (float) rand() / RAND_MAX;
    this->triggerEnergy = triggerEnergy;
    this->life_time = ((float) rand() / RAND_MAX) * 100;
}

Fish::~Fish(){}

void Fish::setColor (const Cairo::RefPtr<Cairo::Context> &cr){
    // Imposto il colore
    cr->set_source_rgba(((float) this->kindness / 100), 0, 0,(float) this->life_bar);
}

Fish* Fish::procreate(Fish* f1){
    this->life_bar -= EN_REPR;
    f1->life_bar -= EN_REPR;
    
    int kidkind;
    int kidspeed;
    float kidtrigger;
    
    if (rand() / RAND_MAX <= MUTATION) {		//possible mutazione
    	kidkind = rand()%101;	
    }
    else {
    	if (rand()%2) {					// scelta del genitore
    		kidkind = this->kindness;
    	}
    	else {
    		kidkind = f1->kindness;
    	}
    }
    
    if (rand() / RAND_MAX <= MUTATION) {
    	kidspeed = 1 + rand()%10;			//è da scegliere il max della speed
    }
    else {
    	if (rand()%2) {
    		kidspeed = this->speed;
    	}
    	else {
    		kidspeed = f1->speed;
    	}
    }

    if (rand() / RAND_MAX <= MUTATION) {
    	kidtrigger = 0.5 + (float) rand() / RAND_MAX / 2;		
    }
    else {
    	if (rand()%2) {
    		kidtrigger = this->triggerEnergy;
    	}
    	else {
    		kidtrigger = f1->triggerEnergy;
    	}
    }
    
    return new Fish(kidkind, kidspeed, kidtrigger);
}

void Fish::shareFood (Fish *f1) {    // ATTENZIONE NON CANCELLA IL CIBO DALLA MAPPA (ma è da fare)
	double food = EN_FOOD;
	
	if ((this->kindness + f1->kindness)>130 && (rand() / RAND_MAX)<=BONUS_RATE) {    //il banco di pesci aumenta il cibo
		food = EN_FOOD*(1 + 3*rand() / RAND_MAX);
	}	

	double enGain = food/2*(f1->life_bar/(f1->life_bar + this->life_bar) + ((double)f1->kindness)/((double)f1->kindness + (double)this->kindness));
	this->life_bar += enGain;
	f1->life_bar += (food - enGain);
	if (this->life_bar>1) {			//CONTROLLA CHE L'ENERGIA DEL PESCE NON SUPERI 1
		this->life_bar = 1;
	}
	if (f1->life_bar>1) {
		f1->life_bar = 1;
	}
}



void Fish::fightFood (Fish *f1) {
	double probability = f1->life_bar / (this->life_bar + f1->life_bar);
	double enLoss;      //energia consumata per combattere (ognuno ne perde un valore diverso)
	
	enLoss = FIGHT_LOSS*(f1->life_bar)/(f1->life_bar + this->life_bar);
	
	// controlliamo se sono morti per combattere prima di mangiare
	f1->life_bar -= (FIGHT_LOSS - enLoss);
	if (f1->life_bar<0) {f1->life_bar=0;}
	this->life_bar -= enLoss;
	if (this->life_bar<0) {this->life_bar=0;}		
	
	// faccio mangiare chi vince se è ancora vivo, altrimenti l'altro se è ancora vivo, altrimenti nessuno (il cibo dovrebbe rimanere sulla mappa)
	
	if ((rand() / RAND_MAX) <= probability) {
		if (f1->life_bar>0) {
			f1->life_bar += EN_FOOD; 
		}
		else if (this->life_bar>0) {
			this->life_bar += EN_FOOD;
		}	
	}
	
	else {
		if (this->life_bar>0) {	
			this->life_bar += EN_FOOD; 
		}
		else if (f1->life_bar>0) {
			f1->life_bar += EN_FOOD;
		}
	}
	
}

void Fish::eat(){
    this->life_bar += EN_FOOD;
    if (this->life_bar>1) {			//CONTROLLA CHE L'ENERGIA DEL PESCE NON SUPERI 1
	this->life_bar = 1;
    }
}


