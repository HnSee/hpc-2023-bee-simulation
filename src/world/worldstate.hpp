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
  WorldState(std::unique_ptr<WorldMap> map, ChunkBounds bounds,
             ChunkBounds worldBounds, int globalChunkCount, int chunkIndex)
      : map(std::move(map)), bounds(bounds), worldBounds(worldBounds),
        globalChunkCount(globalChunkCount), chunkIndex(chunkIndex) {}

  PointTree<double, Agent> agents;
  std::unique_ptr<WorldMap> map;
  Configuration config;
  int day = 0;

  void init(const std::vector<AgentTemplate> &initialAgents);
  void tick();
  std::vector<PointValue<double, Agent>> getAgentsOutOfBounds();

private:
  ChunkBounds bounds;
  ChunkBounds worldBounds;
  int globalChunkCount;
  int chunkIndex;

  std::vector<std::pair<int, std::shared_ptr<Agent>>> agentsForChunkTransfer;

  int getTargetChunk(Coordinates<double> point) const;
};

#endif