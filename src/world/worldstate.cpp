#include "worldstate.hpp"
#include "../agents/bee.hpp"
#include "../agents/flower.hpp"
#include "../agents/hive.hpp"

#include <spdlog/spdlog.h>

void WorldState::init(const std::vector<AgentTemplate> &initialAgents) {
  for (const AgentTemplate &a : initialAgents) {
    std::shared_ptr<Agent> newAgent;

    switch (a.agentType) {
    case AgentType::Hive:
      newAgent = std::make_shared<Hive>(this, Coordinates<double>{0, 0});
      std::dynamic_pointer_cast<Hive>(newAgent)->init(40000);
      break;
    case AgentType::Flower:
      newAgent = std::make_shared<Flower>(this, Coordinates<double>{0, 0});
      std::dynamic_pointer_cast<Flower>(newAgent)->init(10, 20, 30);
      break;
    case AgentType::Bee:
      newAgent = std::make_shared<Bee>(this, Coordinates<double>{0, 0});
      break;
    }

    PointValue<double, Agent> newAgentAtPoint(a.position, newAgent);
    this->agents.add(newAgentAtPoint);
  }

  this->agents.rebalance();
}

std::vector<AgentToTransfer> WorldState::tick() {
  std::vector<AgentToTransfer> agentsForChunkTransfer;
  std::vector<AgentToMove> agentsToMove;

  this->agents.traverse([this, &agentsForChunkTransfer,
                         &agentsToMove](const PointValue<double, Agent> &pv) {
    // Update phase
    pv.value->update();

    // Move phase
    Coordinates<double> newPosition = pv.value->move();

    // MOVE TO AGENTS
    newPosition.clamp(this->worldBounds.xMin, this->worldBounds.xMax,
                      this->worldBounds.yMin, this->worldBounds.yMax);

    if (pv.point != newPosition) {
      int targetChunk = this->getTargetChunk(newPosition);
      if (targetChunk != this->chunkIndex) {
        AgentToTransfer agentToTransfer{targetChunk, pv.value};
        agentsForChunkTransfer.push_back(agentToTransfer);
      }
    } else {
      AgentToMove agentToMove{pv.point, pv.value};
      agentsToMove.push_back(agentToMove);
    }
  });

  // Remove out of chunk agents phase
  for (auto &a : agentsForChunkTransfer) {
    PointValue<double, Agent> pvToRemove(a.second->getPosition(), a.second);
    this->agents.removeByPointValue(pvToRemove);
  }

  // Movement update phase
  for (auto &a : agentsToMove) {
    PointValue<double, Agent> pvToRemove(a.first, a.second);
    this->agents.removeByPointValue(pvToRemove);
    PointValue<double, Agent> pvToAdd(a.second->getPosition(), a.second);
    this->agents.add(pvToAdd);
  }

  // Rebalance phase
  this->agents.rebalance();

  return agentsForChunkTransfer;
}

int WorldState::getTargetChunk(Coordinates<double> point) const {
  return calcualteChunkIndexOfPoint(this->worldBounds.xMax,
                                    this->worldBounds.yMax,
                                    this->globalChunkCount, point.x, point.y);
}
