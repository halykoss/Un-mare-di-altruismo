#include "initializer.h"
#include "../tile/tile.h"
#include "../tile/food/food.h"
#include "../tile/fish/fish.h"
#include "../utils.h"
#include <iostream>
#include <chrono>
#include <list>
#include <iterator>
#include <mutex>
#include <ctime>

using namespace std;

void Initializer::move_fish(int i, int j, int sign1, int sign2)
{
    // Genero a caso la direzione da cui il pesce guarda il campo
    // Cerco una casella libera, muovo il pesce, aggiorno il costo della mossa
    // Se è rimasto senza vita il pesce muore
    bool moved = false;
    for (int k = (sign1 ? -1 : 1) * 1; (((sign1 ? 1 : -1) * k) <= 1) && !moved; (sign1 ? k++ : k--))
    {
        for (int h = (sign2 ? 1 : -1) * 1; (((sign2 ? 1 : -1) * h) <= 1) && !moved; (sign2 ? h++ : h--))
        {
            if (i + k < MAP_SIZE_W && i + k >= 0 && j + h < MAP_SIZE_H && j + h >= 0 && ((*map)[i + k][j + h] == NULL))
            {
                (*map)[i + k][j + h] = (*map)[i][j];
                (*map)[i][j] = nullptr;
                Fish *p = dynamic_cast<Fish *>((*map)[i + k][j + h]);
                p->life_bar -= DECAY_TIME;
                if (p->life_bar <= 0.0)
                {
                    p->died = true;
                    (*map)[i + k][j + h] = nullptr;
                    CURR_FISH--;
                }
                moved = true;
            }
            else if (i + k < MAP_SIZE_W && i + k >= 0 && j + h < MAP_SIZE_H && j + h >= 0 && ((*map)[i + k][j + h])->t == Tile::type::food)
            {
                moved = true;
                (*map)[i + k][j + h] = (*map)[i][j];
                (*map)[i][j] = nullptr;
                Fish *v = dynamic_cast<Fish *>((*map)[i + k][j + h]);
                v->life_bar = 1;
                v->life_bar -= DECAY_TIME;
                CURR_FOOD--;
            }
        }
    }
}

void Initializer::shareFoodAction(int i, int j)
{
    int sign1 = rand() % 2;
    int sign2 = rand() % 2;
    bool trov = false;
    for (int z = 1; z <= SENSOR_RADIUS; z++)
    {
        for (int k = (sign1 ? -1 : 1) * z; (((sign1 ? 1 : -1) * k) <= z) && !trov; (sign1 ? k++ : k--))
        {
            for (int h = (sign2 ? 1 : -1) * z; (((sign2 ? 1 : -1) * h) <= z) && !trov; (sign2 ? h++ : h--))
            {
                if (i + k < MAP_SIZE_W && i + k >= 0 && j + h < MAP_SIZE_H && j + h >= 0)
                {
                    if (((*map)[i + k][j + h]) != NULL && ((*map)[i + k][j + h])->t == Tile::type::fish)
                    {
                        trov = true;
                        Fish *f = (Fish *)((*map)[i][j]);
                        Fish *f1 = (Fish *)((*map)[i + k][j + h]);
                        f->shareFood(f1);
                    }
                }
            }
        }
    }
    if (!trov)
    {
        Fish *f = (Fish *)((*map)[i][j]);
        f->updateEnergy();
    }
}

bool Initializer::can_move_again()
{
    bool remain = false;
    for (int i = 0; i < MAP_SIZE_W; i++)
    {
        for (int j = 0; j < MAP_SIZE_H; j++)
        {
            if (((*map)[i][j]) != NULL && ((*map)[i][j])->t == Tile::type::fish)
            {
                remain = true;
                Fish *p = dynamic_cast<Fish *>((*map)[i][j]);
                p->moved = 0;
            }
        }
    }
    return remain;
}

bool Initializer::checkFood(int i, int j, int *posx, int *posy)
{
    bool trov = false;
    int sign1 = rand() % 2;
    int sign2 = rand() % 2;
    for (int z = 1; z <= SENSOR_RADIUS; z++)
    {
        for (int k = (sign1 ? -1 : 1) * z; (((sign1 ? 1 : -1) * k) <= z) && !trov; (sign1 ? k++ : k--))
        {
            for (int h = (sign2 ? 1 : -1) * z; (((sign2 ? 1 : -1) * h) <= z) && !trov; (sign2 ? h++ : h--))
            {
                if (i + k < MAP_SIZE_W && i + k >= 0 && j + h < MAP_SIZE_H && j + h >= 0)
                {
                    if (((*map)[i + k][j + h]) != NULL && ((*map)[i + k][j + h])->t == Tile::type::food)
                    {
                        trov = true;
                        if (h > 0)
                        {
                            *posy = 1;
                        }
                        else if (h < 0)
                        {
                            *posy = -1;
                        }
                        if (k > 0)
                        {
                            *posx = 1;
                        }
                        else if (k < 0)
                        {
                            *posx = -1;
                        }
                    }
                }
            }
        }
    }
    return trov;
}

Initializer::Initializer(Tile *(*mapInit)[MAP_SIZE_W][MAP_SIZE_H])
{
    this->map = mapInit;
    // Inserisco i pesci nella mappa
    for (int i = 0; i < NUN_OF_FISH; i++)
    {
        int r1 = rand() % MAP_SIZE_W, r2 = rand() % MAP_SIZE_H;
        if ((*map)[r1][r2] == nullptr)
        {
            (*map)[r1][r2] = new Fish{(int)((float)100 * rand() / RAND_MAX)};
            list_of_fish.push_front(dynamic_cast<Fish *>((*map)[r1][r2]));
            CURR_FISH++;
        }
        else
        {
            i--;
        }
    }
    // Inserisco il cibo sulla mappa
    for (int i = 0; i < NUM_OF_FOOD; i++)
    {
        int r1 = rand() % MAP_SIZE_W, r2 = rand() % MAP_SIZE_H;
        if ((*map)[r1][r2] == nullptr)
        {
            (*map)[r1][r2] = new Food;
            CURR_FOOD++;
        }
        else
        {
            i--;
        }
    }
}

Fish* Initializer::procreate(Fish *v, int i, int j)
{
    if (v->life_bar >= v->triggerEnergy)
    {
        int sign1 = rand() % 2;
        int sign2 = rand() % 2;
        bool trov = false;
        for (int z = 1; z <= SENSOR_RADIUS; z++)
        {
            for (int k = (sign1 ? -1 : 1) * z; (((sign1 ? 1 : -1) * k) <= z) && !trov; (sign1 ? k++ : k--))
            {
                for (int h = (sign2 ? 1 : -1) * z; (((sign2 ? 1 : -1) * h) <= z) && !trov; (sign2 ? h++ : h--))
                {
                    if (i + k < MAP_SIZE_W && i + k >= 0 && j + h < MAP_SIZE_H && j + h >= 0)
                    {
                        if (((*map)[i + k][j + h]) != NULL && ((*map)[i + k][j + h])->t == Tile::type::fish)
                        {
                            Fish *f1 = (Fish *)((*map)[i + k][j + h]);
                            if (f1->life_bar >= f1->triggerEnergy)
                            {
                                trov = true;
                                return v->procreate(f1);
                            }
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

bool Initializer::updateMap(mutex *mx)
{
    srand(time(NULL));
    mx->lock();
    for (int i = 0; i < NUM_OF_FOOD_PER_SPAWN; i++)
    {
        int r1 = rand() % MAP_SIZE_W, r2 = rand() % MAP_SIZE_H;
        if ((*map)[r1][r2] == nullptr)
        {
            (*map)[r1][r2] = new Food;
            CURR_FOOD++;
        }
    }
    // Scorro la mappa
    int count = rand();
    for (int i = 0; i < MAP_SIZE_W; i++)
    {
        for (int j = 0; j < MAP_SIZE_H; j++)
        {
            // Se nella casella c'è un pesce
            if (((*map)[i][j]) != NULL && ((*map)[i][j])->t == Tile::type::fish)
            {
                Fish *v = dynamic_cast<Fish *>((*map)[i][j]);
                if (v->moved)
                {
                    continue;
                }
                
                if(v->life_time <= v->curr_life){
                    (*map)[i][j] = nullptr;
                    CURR_FISH--;
                    continue;
                }

                int posy = 0;
                int posx = 0;
                // Guardo il raggio visivo e cerco di muovermi
                bool trov = checkFood(i, j, &posx, &posy);
                // Se non ha trovato del cibo si muove in un posto libero
                if (!trov)
                {
                    move_fish(i, j, rand() % 2, rand() % 2);
                }
                else
                {
                    // Se ha trovato il cibo, mangia
                    if (((*map)[i + posx][j + posy]) != NULL && ((*map)[i + posx][j + posy])->t == Tile::type::food)
                    {
                        // Guardo il raggio visivo e cerco un altro pesce
                        shareFoodAction(i, j);
                        (*map)[i + posx][j + posy] = (*map)[i][j];
                        (*map)[i][j] = nullptr;
                        //cout << "Cibo condiviso" << endl;
                        //v->life_bar = 1;
                        v->life_bar -= DECAY_TIME;
                        CURR_FOOD--;
                    }
                    // Se la casella è vuota, il pesce si muove
                    else if (((*map)[i + posx][j + posy]) == NULL)
                    {
                        (*map)[i + posx][j + posy] = (*map)[i][j];
                        (*map)[i][j] = nullptr;
                        Fish *p = dynamic_cast<Fish *>((*map)[i + posx][j + posy]);
                        p->life_bar -= DECAY_TIME;
                        if (p->life_bar <= 0.0)
                        {
                            p->died = true;
                            (*map)[i + posx][j + posy] = nullptr;
                            CURR_FISH--;
                        }
                    }
                    // Se trova occupato, il pesce si muove in una casella vuota
                    else
                    {
                        move_fish(i, j, rand() % 2, rand() % 2);
                    }
                }
                v->moved = 1;
                v->curr_life += 1;
            }
        }
    }
    // Azzero le mosse dei pesci
    bool remain = can_move_again();
    mx->unlock();

    return !remain;
}

Initializer::~Initializer()
{
}