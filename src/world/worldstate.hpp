#ifndef BEESIMULATION_WORLD_WORLDSTATE_H
#define BEESIMULATION_WORLD_WORLDSTATE_H

#include "../agents/agent.hpp"
#include "../utils/point_tree.hpp"
#include "configuration.hpp"
#include "generator.hpp"
#include <memory>

class WorldState {
public:
  WorldState(WorldMap *map) : map(map) {}
  PointTree<double, Agent> agents;
  WorldMap *map;
  Configuration config;
  int day = 0;

  void tick();
};

#endif