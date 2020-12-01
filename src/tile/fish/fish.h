#ifndef FISH_H
#define FISH_H
#include "../tile.h"

const double FIGHT_LOSS = 1;		//coeff perdita energia in lotta
const double EN_FOOD = 0.33;		//energia del cibo base
const double EN_REPR = 0.40;        	//costo riproduzione
const double BONUS_RATE = 0.20;		//probabilità di trovare un banco


// Astrae un pesce
class Fish : public Tile
{
public:
    Fish(int kindness, int speed = 5); // messi settati implicitamente per non cambiare tutto il codice prima
    virtual ~Fish();
    // Colore della casella
    void setColor(const Cairo::RefPtr<Cairo::Context> &cr) override;
    // Divisione cibo
    void shareFood(Fish *f1);
    // Scontro per il cibo
    void fightFood(Fish *f1);
    // Mangia da solo
    void eat();
    Fish* procreate(Fish* f1);
    // Vita del pesce
    double life_bar = 1.0;
    // Se il pesce si è mosso
    int moved = 0;
    // Se il pesce è vivo o morto
    bool died = false;
    // Grado di altruismo
    int kindness;
    // Velocità pesce (non deve essere minore di 1)
    int speed = 1;
    // double trigger energy
    float triggerEnergy;
    // Durate della vita del pesce
    int life_time;
    // Tempo di vita corrente
    int curr_life = 0;
protected:
    //double enFood = 0.33; // energia che dà il cibo 	L'HO MESSA GLOBALE
};

#endif
