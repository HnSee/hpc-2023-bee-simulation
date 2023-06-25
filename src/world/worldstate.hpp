#ifndef BEESIMULATION_WORLD_WORLDSTATE_H
#define BEESIMULATION_WORLD_WORLDSTATE_H

#include "../agents/agent.hpp"
#include "../utils/point_tree.hpp"
#include "configuration.hpp"
#include "generator.hpp"
#include <cmath>
#include <memory>
#include <utility>

class WorldState {
public:
  WorldState(WorldMap *map, int xMin, int xMax, int yMin, int yMax)
      : map(map), xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax) {}

  PointTree<double, Agent> agents;
  WorldMap *map;
  Configuration config;
  int day = 0;

  void tick();
  void move(std::shared_ptr<Agent> agent, Coordinates<double> to);
  std::vector<PointValue<double, Agent>> getAgentsOutOfBounds();

private:
  int xMin;
  int xMax;
  int yMin;
  int yMax;
};

/// @brief Struct for representing closed-open chunk borders with [xMin, xMax)
/// and [yMin, yMax)
struct ChunkBounds {
  int xMin;
  int xMax;
  int yMin;
  int yMax;
};

std::pair<int, int> calculateClosestDivisors(int n);

ChunkBounds calcualteChunkBounds(int areaMaxX, int areaMaxY, int chunkCount,
                                 int chunkIndex);

#endif