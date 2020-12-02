#include "initializer.h"
#include "../tile/tile.h"
#include "../tile/food/food.h"
#include "../tile/fish/fish.h"
#include "../utils.h"
#include <iostream>
#include <chrono>
#include <list>
#include <random>
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
                v->eat();
                v->life_bar -= DECAY_TIME;
                CURR_FOOD--;
            }
        }
    }
}

void Initializer::shareorFightFoodAction(int i, int j)
{
    int sign1 = dist(mt);
    int sign2 = dist(mt);
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
                        if (f->kindness + f1->kindness >= 1.0)
                        {
                            f->shareFood(f1);
                            CURR_FOOD--;
                        }
                        else
                        {
                            bool eat = f->fightFood(f1);
                            if (eat)
                            {
                                CURR_FOOD--;
                            }
                        }

                        if (f->life_bar <= 0.0)
                        {
                            CURR_FISH--;
                            (*map)[i][j] = nullptr;
                        }

                        if (f1->life_bar <= 0.0)
                        {
                            CURR_FISH--;
                            ((*map)[i + k][j + h]) = nullptr;
                        }
                    }
                }
            }
        }
    }
    if (!trov)
    {
        Fish *f = (Fish *)((*map)[i][j]);
        f->eat();
        CURR_FOOD--;
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
    int sign1 = dist(mt) % 2;
    int sign2 = dist(mt) % 2;
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
        int r1 = dist(mt) % MAP_SIZE_W, r2 = dist(mt) % MAP_SIZE_H;
        if ((*map)[r1][r2] == nullptr)
        {
            (*map)[r1][r2] = new Fish{(int)((float)100 * dist(mt) / MAX_RAND_VALUE)};
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
        int r1 = dist(mt) % MAP_SIZE_W, r2 = dist(mt) % MAP_SIZE_H;
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

Fish *Initializer::procreate(Fish *v, int i, int j)
{
    if (v->life_bar >= v->triggerEnergy)
    {
        int sign1 = dist(mt) % 2;
        int sign2 = dist(mt) % 2;
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

void Initializer::locate(Fish *n, int i, int j)
{
    for (int z = 1; z <= 3; z++)
    {
        for (int k = -1 * z; k < z; k++)
        {
            for (int h = z; h > (-1 * z); h--)
            {
                if (i + k < MAP_SIZE_W && i + k >= 0 && j + h < MAP_SIZE_H && j + h >= 0)
                {
                    if (((*map)[i + k][j + h]) == NULL)
                    {
                        (*map)[i + k][j + h] = n;
                        CURR_FISH++;
                        return;
                    }
                }
            }
        }
    }
}

bool Initializer::updateMap(mutex *mx)
{
    mx->lock();
    for (int i = 0; i < NUM_OF_FOOD_PER_SPAWN; i++)
    {
        int r1 = dist(mt) % MAP_SIZE_W, r2 = dist(mt) % MAP_SIZE_H;
        if ((*map)[r1][r2] == nullptr)
        {
            (*map)[r1][r2] = new Food;
            CURR_FOOD++;
        }
    }
    // Scorro la mappa
    int count = dist(mt);
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

                if (v->life_time <= v->curr_life)
                {
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
                    move_fish(i, j, dist(mt) % 2, dist(mt) % 2);
                }
                else
                {
                    // Se ha trovato il cibo, mangia
                    if (((*map)[i + posx][j + posy]) != NULL && ((*map)[i + posx][j + posy])->t == Tile::type::food)
                    {
                        // Guardo il raggio visivo e cerco un altro pesce
                        shareorFightFoodAction(i, j);
                        (*map)[i + posx][j + posy] = (*map)[i][j];
                        (*map)[i][j] = nullptr;
                        v->life_bar -= DECAY_TIME;
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
                        move_fish(i, j, dist(mt) % 2, dist(mt) % 2);
                    }
                }
                Fish *child = procreate(v, i, j);
                if (child != NULL)
                {
                    locate(child, i, j);
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
