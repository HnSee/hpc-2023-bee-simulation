#include "worldstate.hpp"
#include "../agents/bee.hpp"
#include "../agents/flower.hpp"
#include "../agents/hive.hpp"

void WorldState::init(const std::vector<AgentTemplate> &initialAgents) {
  for (const AgentTemplate &a : initialAgents) {
    std::shared_ptr<Agent> newAgent;

    switch (a.agentType) {
    case AgentType::Hive:
      newAgent = std::make_shared<Hive>(*this, Coordinates<double>{0, 0});
      break;
    case AgentType::Flower:
      newAgent = std::make_shared<Flower>(*this, Coordinates<double>{0, 0});
      break;
    case AgentType::Bee:
      newAgent = std::make_shared<Bee>(*this, Coordinates<double>{0, 0});
      break;
    }

    // newAgent->init();

    PointValue<double, Agent> newAgentAtPoint(a.position, newAgent);
    this->agents.add(newAgentAtPoint);
  }

  this->agents.rebalance();
}

void WorldState::tick() {
  this->agents.traverse([this](const PointValue<double, Agent> &pv) {
    // Move phase
    Coordinates<double> newPosition = pv.value->move();
    if (pv.point != newPosition) {
      int targetChunk = this->getTargetChunk(newPosition);
      if (targetChunk != this->chunkIndex) {
        std::pair<int, std::shared_ptr<Agent>> agentToTransfer{targetChunk,
                                                               pv.value};
        this->agentsForChunkTransfer.push_back(std::move(agentToTransfer));
         this->agents.removeByPointValue(
            pv);
      }
    }

    // Update phase
    pv.value->update();
  });
}

int WorldState::getTargetChunk(Coordinates<double> point) const {
  return calcualteChunkIndexOfPoint(this->worldBounds.xMax,
                                    this->worldBounds.yMax,
                                    this->globalChunkCount, point.x, point.y);
}
