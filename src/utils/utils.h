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
    static int SIZE_CELL_W;
    static int SIZE_CELL_H;
    static int WINDOW_SIZE_W;
    static int WINDOW_SIZE_H;
};

#endif
