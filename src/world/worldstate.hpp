#ifndef BEESIMULATION_WORLD_WORLDSTATE_H
#define BEESIMULATION_WORLD_WORLDSTATE_H

#include "../utils/two_d_tree.hpp"
#include "generator.hpp"
#include <memory>

class WorldState {
public:
  WorldState(WorldMap map);

private:
  TwoDTree<double, double> agents;
  WorldMap map;
};

#endif