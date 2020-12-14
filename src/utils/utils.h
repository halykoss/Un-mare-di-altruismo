#ifndef UTILS_H
#define UTILS_H
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

using namespace std;

using json = nlohmann::json;

const int MAP_SIZE_W = 45;
const int MAP_SIZE_H = 45;
const int SENSOR_RADIUS = 2;
const int REFRESH_TIME = 10;
const double MAX_RAND_VALUE = 100.0;

class Utils
{
public:
    static int NUM_OF_FISH;
    static int NUM_OF_FOOD;
    static int NUM_OF_FOOD_PER_SPAWN;
    static double DECAY_TIME;
    static void get_settings_from_json();
    static double FIGHT_LOSS; //= 1;    //coeff perdita energia in lotta
    static double EN_FOOD;    //= 0.33;    //energia del cibo base
    static double EN_REPR;    //= 0.40;    //costo riproduzione
    static double BONUS_RATE; //= 0.20; //probabilità di trovare un banco
    static double MUTATION;   //= 0.10;   //probabilità mutazione
    static int SIZE_CELL_W;
    static int SIZE_CELL_H;
    static int WINDOW_SIZE_W;
    static int WINDOW_SIZE_H;
    static bool SHARE_ACTIVE;
    static bool FIGHT_ACTIVE;
};

#endif
