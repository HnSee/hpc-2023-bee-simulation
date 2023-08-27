#include "agent_type.hpp"

std::ostream &operator<<(std::ostream &os, const AgentType &agentType) {
  switch (agentType) {
  case AgentType::Bee:
    return os << "Bee";
  case AgentType::Flower:
    return os << "Flower";
  case AgentType::Hive:
    return os << "Hive";
  }

  return os << static_cast<int>(agentType);
}