#ifndef BEESIMULATION_WORLD_WORLDSTATE_H
#define BEESIMULATION_WORLD_WORLDSTATE_H

#include "../agents/agent.hpp"
#include "../utils/point_tree.hpp"
#include "chunking.hpp"
#include "configuration.hpp"
#include "generator.hpp"
#include "seeding.hpp"
#include <cmath>
#include <memory>
#include <utility>
#include <vector>

class WorldState {
public:
  WorldState(std::unique_ptr<WorldMap> map, ChunkBounds bounds)
      : map(std::move(map)), bounds(bounds) {}

  PointTree<double, Agent> agents;
  std::unique_ptr<WorldMap> map;
  Configuration config;
  int day = 0;

  void init(std::vector<AgentTemplate> &agents);
  void tick();
  void move(std::shared_ptr<Agent> agent, Coordinates<double> to);
  std::vector<PointValue<double, Agent>> getAgentsOutOfBounds();

private:
  ChunkBounds bounds;
};

#endif