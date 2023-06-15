#ifndef BEESIMULATION_WORLD_WORLDSTATE_H
#define BEESIMULATION_WORLD_WORLDSTATE_H

#include "../utils/point_tree.hpp"
#include "generator.hpp"
#include <memory>

class WorldState {
public:
  WorldState(WorldMap map);

private:
  PointTree<double, double> agents;
  WorldMap map;
};

#endif