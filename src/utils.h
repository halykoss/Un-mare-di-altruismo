#ifndef UTILS_H
#define UTILS_H
    const int MAP_SIZE_W = 45;
    const int MAP_SIZE_H = 45;
    const int NUN_OF_FISH = 100;
    const int NUM_OF_FOOD = 100;
    //const double SPAWN_TIME = 0.05;
    const int NUM_OF_FOOD_PER_SPAWN = 5;
    const int SENSOR_RADIUS = 2;
    const int REFRESH_TIME = 100;
    const int WINDOW_SIZE_W = 900;
    const int WINDOW_SIZE_H = 900;
    extern int CURR_FISH;
    extern int CURR_FOOD;
    const double DECAY_TIME = 0.01;
    const int SIZE_CELL_W = WINDOW_SIZE_W / MAP_SIZE_W;
    const int SIZE_CELL_H = WINDOW_SIZE_H / MAP_SIZE_H;
#endif
