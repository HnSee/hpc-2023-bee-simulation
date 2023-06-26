#include "worldstate.hpp"
#include "../agents/bee.hpp"
#include "../agents/flower.hpp"
#include "../agents/hive.hpp"

void WorldState::init(std::vector<AgentTemplate> &agents) {
  for (AgentTemplate &a : agents) {
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
  }
}

void WorldState::tick() {
  this->agents.traverse(
      [](const PointValue<double, Agent> &pv) { pv.value->update(); });
}
