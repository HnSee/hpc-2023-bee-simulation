#include <random>

#include "chunking.hpp"
#include "seeding.hpp"

std::vector<AgentTemplate> generateInitialAgents(int xMin, int xMax, int yMin,
                                                 int yMax,
                                                 SeedingConfiguration &config) {
  std::vector<AgentTemplate> result;

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<double> unfX(xMin, xMax);
  std::uniform_real_distribution<double> unfY(yMin, yMax);

  if (config.hiveCount == 1) {
    double posX = (xMin + xMax) / 4 * 3;
    double posY = (yMin + yMax) / 4 * 3;

    result.emplace_back(posX, posY, AgentType::Hive);
  } else {
    for (int i = 0; i < config.hiveCount; ++i) {
      double randomX = unfX(rng);
      double randomY = unfY(rng);

      result.emplace_back(randomX, randomY, AgentType::Hive);
    }
  }

  for (int i = 0; i < config.flowerCount; ++i) {
    double randomX = unfX(rng);
    double randomY = unfY(rng);

    result.emplace_back(randomX, randomY, AgentType::Flower);
  }

  return result;
}

std::vector<std::vector<AgentTemplate>>
partitionInitialAgentsIntoChunks(std::vector<AgentTemplate> &initialAgents,
                                 int areaMaxX, int areaMaxY, int chunkCount) {
  std::vector<std::vector<AgentTemplate>> result(chunkCount);
  std::vector<ChunkBounds> chunkBounds(chunkCount);

  for (std::size_t i = 0; i < chunkBounds.size(); ++i) {
    chunkBounds[i] = calcualteChunkBounds(areaMaxX, areaMaxY, chunkCount, i);
  }

  for (std::size_t i = 0; i < initialAgents.size(); ++i) {
    for (std::size_t j = 0; j < chunkBounds.size(); ++j) {
      if (initialAgents[i].position.x >= chunkBounds[j].xMin &&
          initialAgents[i].position.x < chunkBounds[j].xMax &&
          initialAgents[i].position.y >= chunkBounds[j].yMin &&
          initialAgents[i].position.y < chunkBounds[j].yMax) {
        result[j].push_back(initialAgents[i]);
      }
    }
  }

  return result;
}