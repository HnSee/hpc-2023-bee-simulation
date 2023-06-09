#ifndef BEESIMULATION_WORLD_WORLDSTATE_H
#define BEESIMULATION_WORLD_WORLDSTATE_H

#include "../extern/kdtree/kdtree.hpp"
#include "generator.hpp"
#include <memory>

class WorldState {
public:
  WorldState(std::unique_ptr<WorldMap> map);

private:
  Kdtree::KdTree agents;
  std::unique_ptr<WorldMap> map;
}

#endif