#include "utils.h"

int Utils::NUM_OF_FOOD = 100;
int Utils::NUM_OF_FISH = 300;
int Utils::NUM_OF_FOOD_PER_SPAWN = 5;
int Utils::WINDOW_SIZE_W = 1000;
int Utils::WINDOW_SIZE_H = 1000;
int Utils::SIZE_CELL_W = WINDOW_SIZE_W / MAP_SIZE_W;
int Utils::SIZE_CELL_H = WINDOW_SIZE_H / MAP_SIZE_H;
double Utils::DECAY_TIME = 0.01;

void Utils::get_settings_from_json()
{
    std::ifstream file("settings.json");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    auto j3 = json::parse(content);
    NUM_OF_FOOD = j3["NUM_OF_FOOD"]["value"];
    NUM_OF_FISH = j3["NUM_OF_FISH"]["value"];
    NUM_OF_FOOD_PER_SPAWN = j3["NUM_OF_FOOD_PER_SPAWN"]["value"];
    DECAY_TIME = j3["DECAY_TIME"]["value"];
}