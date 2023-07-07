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
  std::vector<AgentToMove> agentsToMove;

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

  spdlog::debug(
      "Update time: {}µs",
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start)
          .count());

  start = std::chrono::high_resolution_clock::now();

  // Move phase
  this->agents.traverse([this, &agentsForChunkTransfer,
                         &agentsToMove](const PointValue<double, Agent> &pv) {
    Coordinates<double> newPosition = pv.value->move(this->worldBounds);

    if (pv.point != newPosition) {
      int targetChunk = this->getTargetChunk(newPosition);
      if (targetChunk != this->chunkIndex) {
        AgentToTransfer agentToTransfer{targetChunk, pv.point, pv.value};
        agentsForChunkTransfer.push_back(agentToTransfer);
      } else {
        AgentToMove agentToMove(pv.point, pv.value);
        agentsToMove.push_back(agentToMove);
      }
    }
  });

  stop = std::chrono::high_resolution_clock::now();
  spdlog::debug(
      "Move time: {}µs",
      std::chrono::duration_cast<std::chrono::microseconds>(stop - start)
          .count());

  // Remove out of chunk agents phase
  for (auto &a : agentsForChunkTransfer) {
    PointValue<double, Agent> pvToRemove(a.formerPosition, a.agent);
    this->agents.removeByPointValue(pvToRemove);
  }

  spdlog::debug("[{}] Moving {} agents (currently {} agents total)",
                this->chunkIndex, agentsToMove.size(), this->agents.count());

  // Movement update phase
  for (auto &a : agentsToMove) {
    std::size_t prevCount = this->agents.count();
    PointValue<double, Agent> pvToRemove(a.first, a.second);
    this->agents.removeByPointValue(pvToRemove);

    if (prevCount - 1 != this->agents.count()) {
      spdlog::error("Previous count: {}, current count: {}", prevCount,
                    this->agents.count());
      std::ofstream f("out.csv");
      f << this->agents.toDot();
      f.close();
    }

    PointValue<double, Agent> pvToAdd(a.second->getPosition(), a.second);
    this->agents.add(pvToAdd);
  }

  spdlog::debug("[{}] {} agents moved (currently {} agents total)",
                this->chunkIndex, agentsToMove.size(), this->agents.count());

  // Rebalance phase
  this->agents.rebalance();

  return agentsForChunkTransfer;
}

int WorldState::getTargetChunk(Coordinates<double> point) const {
  return calcualteChunkIndexOfPoint(this->worldBounds.xMax,
                                    this->worldBounds.yMax,
                                    this->globalChunkCount, point.x, point.y);
}
