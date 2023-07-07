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

struct AgentToTransfer {
  int targetChunk;
  Coordinates<double> formerPosition;
  std::shared_ptr<Agent> agent;
};

using AgentToMove = std::pair<Coordinates<double>, std::shared_ptr<Agent>>;

class WorldState {
public:
  WorldState(std::unique_ptr<WorldMap> map, ChunkBounds bounds,
             ChunkBounds worldBounds, int globalChunkCount, int chunkIndex)
      : map(std::move(map)), bounds(bounds), worldBounds(worldBounds),
        globalChunkCount(globalChunkCount), chunkIndex(chunkIndex) {}

  PointTree<double, Agent> agents;
  std::unique_ptr<WorldMap> map;
  Configuration config;
  int day = 0;

  void init(const std::vector<AgentTemplate> &initialAgents);
  std::vector<AgentToTransfer> tick();

private:
  ChunkBounds bounds;
  ChunkBounds worldBounds;
  int globalChunkCount;
  int chunkIndex;

  int getTargetChunk(Coordinates<double> point) const;
};

#endif