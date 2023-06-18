#ifndef BEESIMULATION_AGENTS_VAR_H
#define BEESIMULATION_AGENTS_VAR_H

#include "agent.hpp"

// percentage that a scout continues after he didn't find any foodsource
extern const int scoutindurance;

// one tick = 1sec -> one day 12h -> 43200 ticks
extern const int daylength;

// speed of bee
extern const float speed;

// current day
extern int day; // 210 days a year

// foreward declaration of classes
// class agent;
// class hive;
// class flower;
// class bee;

// datastructure of agents
extern std::vector<Agent *> agents;

// remove current element of vector
extern bool removeagent;

#endif