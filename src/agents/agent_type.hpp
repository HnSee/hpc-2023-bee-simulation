#ifndef BEESIMULATION_AGENTS_AGENT_TYPE_H
#define BEESIMULATION_AGENTS_AGENT_TYPE_H

#include <ostream>

enum class AgentType { Bee, Flower, Hive };

std::ostream &operator<<(std::ostream &os, const AgentType &agentType);

#endif