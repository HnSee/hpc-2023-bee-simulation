#ifndef BEESIMULATION_AGENTS_HIVE_H
#define BEESIMULATION_AGENTS_HIVE_H

#include "../world/worldstate.hpp"
#include "agent.hpp"

class Hive : public Agent{
public:
  using Agent::Agent;

  // access to the datastructure to spawn bees into
  std::vector<Agent *> *ds;

  // what time of the day it is
  int tickoftheday;

  // all the bees belonging to the hive
  int totalbees;

  // all the collected food
  int totalfood;

  int activebees;

  std::vector<Coordinates<double>> foodsources;

  void init(int totalbees);

  Coordinates<double> move(ChunkBounds worldBounds) override;

  int getsize() const;

  void update() override;

  void add_fs(Coordinates<double> p);

  void rem_fs(Coordinates<double> p);

  Coordinates<double> rand_fs();

  AgentType gettype() const override;
};

#endif