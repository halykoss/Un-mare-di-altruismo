#ifndef FISH_H
#define FISH_H
#include "../tile.h"
// Astrae un pesce
class Fish : public Tile
{
public:
    Fish(int kindness, double mass = 1, int speed = 5); // messi settati implicitamente per non cambiare tutto il codice prima
    virtual ~Fish();
    // Colore della casella
    void setColor(const Cairo::RefPtr<Cairo::Context> &cr) override;
    // Divisione cibo
    void shareFood(Fish *f1);
    // Scontro per il cibo
    void fightFood(Fish *f1);
    // Energia spesa per muoversi
    void movementEnergy(int steps);
    void updateEnergy();
    // Vita del pesce
    double life_bar = 1.0;
    // Se il pesce si è mosso
    int moved = 0;
    // Se il pesce è vivo o morto
    bool died = false;
    // Grado di altruismo
    int kindness;
    // Massa pesce (non deve essere minore di 0.1)
    double mass = 0.1;
    // Velocità pesce (non deve essere minore di 1)
    int speed = 1;

protected:
    double enFood = 0.33; // energia che dà il cibo
};

#endif
