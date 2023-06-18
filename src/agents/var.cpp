#include "var.hpp"

// percentage that a scout continues after he didn't find any foodsource
const int scoutindurance = 20;

// one tick = 1sec -> one day 12h -> 43200 ticks
const int daylength = 43200;

// speed of bee
const float speed = 8;

// current day
int day = 0; // 210 days a year

// foreward declaration of classes
// class agent;
// class hive;
// class flower;
// class bee;

// datastructure of agents
std::vector<Agent *> agents;

// remove current element of vector
bool removeagent = false;