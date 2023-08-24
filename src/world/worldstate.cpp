#include "worldstate.hpp"
#include "../agents/bee.hpp"
#include "../agents/flower.hpp"
#include "../agents/hive.hpp"

#include <chrono>
#include <fstream>
#include <spdlog/spdlog.h>

void WorldState::init(const std::vector<AgentTemplate> &initialAgents) {
  for (const AgentTemplate &a : initialAgents) {
    std::shared_ptr<Agent> newAgent;

    switch (a.agentType) {
    case AgentType::Hive:
      newAgent = std::make_shared<Hive>(
          this, Coordinates<double>{a.position.x, a.position.y});
      std::dynamic_pointer_cast<Hive>(newAgent)->init(40000);
      break;
    case AgentType::Flower:
      newAgent = std::make_shared<Flower>(
          this, Coordinates<double>{a.position.x, a.position.y});
      std::dynamic_pointer_cast<Flower>(newAgent)->init(10, 20, 30);
      break;
    case AgentType::Bee:
      newAgent = std::make_shared<Bee>(
          this, Coordinates<double>{a.position.x, a.position.y});
      break;
    }

    PointValue<double, Agent> newAgentAtPoint(a.position, newAgent);
    this->agents.add(newAgentAtPoint);
  }

  this->agents.rebalance();
}

std::vector<AgentToTransfer> WorldState::tick() {
  std::vector<std::shared_ptr<Agent>> agentsToUpdate;
  std::vector<AgentToTransfer> agentsForChunkTransfer;
  std::vector<PointValue<double, Agent>> agentsOfCurrentChunk;

  auto start = std::chrono::high_resolution_clock::now();

  // Update phase
  this->agents.traverse([&agentsToUpdate](const PointValue<double, Agent> &pv) {
    agentsToUpdate.push_back(pv.value);
  });

  for (auto &a : agentsToUpdate) {
    a->update();
  }

  agentsToUpdate.clear();

  auto stop = std::chrono::high_resolution_clock::now();

  start = std::chrono::high_resolution_clock::now();

  // Move phase
  this->agents.traverse([this, &agentsForChunkTransfer, &agentsOfCurrentChunk](
                            const PointValue<double, Agent> &pv) {
    Coordinates<double> newPosition = pv.value->move(this->worldBounds);

    int targetChunk = this->getTargetChunk(newPosition);
    if (targetChunk != this->chunkIndex) {
      AgentToTransfer agentToTransfer{targetChunk, pv.point, pv.value};
      agentsForChunkTransfer.push_back(agentToTransfer);
    } else {
      PointValue<double, Agent> agentToMove(newPosition, pv.value);
      agentsOfCurrentChunk.push_back(agentToMove);
    }
  });

  // Rebuild tree phase
  this->agents = PointTree<double, Agent>(
      std::make_move_iterator(agentsOfCurrentChunk.begin()),
      std::make_move_iterator(agentsOfCurrentChunk.end()));

  stop = std::chrono::high_resolution_clock::now();

  return agentsForChunkTransfer;
}

int WorldState::getTargetChunk(Coordinates<double> point) const {
  return calcualteChunkIndexOfPoint(this->worldBounds.xMax,
                                    this->worldBounds.yMax,
                                    this->globalChunkCount, point.x, point.y);
}
