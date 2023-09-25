#ifndef BEESIMULATION_WORLD_SEEDING_H
#define BEESIMULATION_WORLD_SEEDING_H

#include <vector>

#include "../agents/agent.hpp"
#include "../agents/agent_type.hpp"

struct SeedingConfiguration {
  int seed;
  int hiveCount;
  int flowerCount;
};

struct AgentTemplate {
  Coordinates<double> position;
  AgentType agentType;

  AgentTemplate(double x, double y, AgentType agentType)
      : position(Coordinates<double>{x, y}), agentType(agentType) {}
};

std::vector<AgentTemplate>
generateInitialAgents(int xMin, int xMax, int yMin, int yMax,
                      const SeedingConfiguration &config, int el, int seed);

std::vector<std::vector<AgentTemplate>>
partitionInitialAgentsIntoChunks(std::vector<AgentTemplate> &initialAgents,
                                 int areaMaxX, int areaMaxY, int chunkCount);

#endif