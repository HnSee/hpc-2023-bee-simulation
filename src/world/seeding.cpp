#include <random>

#include "chunking.hpp"
#include "seeding.hpp"

std::vector<AgentTemplate>
generateInitialAgents( int xMin, int xMax, int yMin, int yMax, const SeedingConfiguration &config, int el, int seed) {
  std::vector<AgentTemplate> result;
  
  std::uniform_real_distribution<double> unif(0, 0.1);
  std::random_device rd;
  std::mt19937 e2(rd());
  std::srand(seed);

  for (int i = 0; i < config.hiveCount; ++i) {
    int randomX = std::rand()%el+unif(e2);
    int randomY = std::rand()%el+unif(e2);

    if(randomX >= xMin && randomX <= xMax && randomY >= yMin && randomY <= yMax){
      result.emplace_back(randomX, randomY, AgentType::Hive);
    }
  }

  for (int i = 0; i < config.flowerCount; ++i) {
    int randomX = std::rand()%el+unif(e2);
    int randomY = std::rand()%el+unif(e2);

    if(randomX >= xMin && randomX <= xMax && randomY >= yMin && randomY <= yMax){
      result.emplace_back(randomX, randomY, AgentType::Flower);
    }
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