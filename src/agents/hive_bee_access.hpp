#ifndef BEESIMULATION_AGENTS_HIVE_BEE_ACCESS_H
#define BEESIMULATION_AGENTS_HIVE_BEE_ACCESS_H

#include <cmath>
#include <iostream>
#include <vector>

#include "var.hpp"

extern int scout;
extern int worker; 

// place where the hive stores the food
class HiveBeeAccess {
public:
  // how much food the hive has
  int foodstore;

  // random access foodsources (maybe change that into kde tree)
  std::vector<Coordinates<double>> foodsources;

  // all the bees with no tasks
  int activebees;

  void addfood(int x);

  // adds foodsource
  void add_fs(Coordinates<double> p);

  // removes foodsource
  void rem_fs(Coordinates<double> p);

  // get random foodsource
  Coordinates<double> rand_fs();
};

// removes agent
bool check_agent_removal(int j);

// one move
void tick(int j);

#endif