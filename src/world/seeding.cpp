#include <random>

#include "chunking.hpp"
#include "seeding.hpp"

std::vector<AgentTemplate> generateInitialAgents(int xMax, int yMax,
                                                 SeedingConfiguration &config) {
  std::vector<AgentTemplate> result;

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<double> unfX(0, xMax);
  std::uniform_real_distribution<double> unfY(0, yMax);

  for (int i = 0; i < config.hiveCount; ++i) {
    double randomX = unfX(rng);
    double randomY = unfY(rng);

    result.emplace_back(randomX, randomY, AgentType::Hive);
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
    int chunkIndex;
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