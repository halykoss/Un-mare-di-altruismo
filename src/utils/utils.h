#ifndef UTILS_H
#define UTILS_H
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

using namespace std;

using json = nlohmann::json;

class Utils
{
public:
    static int NUM_OF_FISH;
    static int NUM_OF_FOOD;
    static int NUM_OF_FOOD_PER_SPAWN;
    static double DECAY_TIME;
    static void get_settings_from_json();
};

const int MAP_SIZE_W = 45;
const int MAP_SIZE_H = 45;
const int SENSOR_RADIUS = 2;
const int REFRESH_TIME = 100;
const int WINDOW_SIZE_W = 405;
const int WINDOW_SIZE_H = 405;
const double MAX_RAND_VALUE = 100.0;
const int SIZE_CELL_W = WINDOW_SIZE_W / MAP_SIZE_W;
const int SIZE_CELL_H = WINDOW_SIZE_H / MAP_SIZE_H;

#endif
