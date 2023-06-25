#ifndef BEESIMULATION_AGENTS_HIVE_H
#define BEESIMULATION_AGENTS_HIVE_H

#include "../world/worldstate.hpp"
#include "agent.hpp"
#include "hive_bee_access.hpp"

class Hive : public Agent {
public:
  using Agent::Agent;

  // place where the food is stored
  HiveBeeAccess *hstore;

  // access to the datastructure to spawn bees into
  std::vector<Agent *> *ds;

  // what time of the day it is
  int tickoftheday;

  // all the bees belonging to the hive
  int totalbees;

  void init(int totalbees, HiveBeeAccess *hstore);

  void move();

  int getsize();

  void update();

  std::string gettype();

  void nuke();
};

#endif