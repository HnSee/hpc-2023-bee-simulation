#ifndef BEESIMULATION_AGENTS_AGENT_H
#define BEESIMULATION_AGENTS_AGENT_H

#include "../utils/point_tree.hpp"
#include "../world/chunking.hpp"
#include "agent_type.hpp"
#include <iostream>

class WorldState;

class Agent {
public:
  Agent() = default;
  Agent(WorldState *state, Coordinates<double> pos) : pos(pos), state(state) {}
  virtual ~Agent() = default;

  // initialization of the Agent, function has to initialize all necessary
  // variables
  virtual void init();

  // movement of the agent
  virtual Coordinates<double> move(ChunkBounds worldBounds);

  // update of agent after move
  virtual void update();

  virtual AgentType gettype() const = 0;

  Coordinates<double> getPosition() const;
  void setPosition(Coordinates<double> newPosition);

  void setState(WorldState *state);

  friend std::ostream &operator<<(std::ostream &output, const Agent &a);

  Coordinates<double> pos;

protected:
  WorldState *state;
};

// returns the position of the agent 1 meter in one direction
Coordinates<double> getmovementvector(Coordinates<double> pos,
                                      Coordinates<double> target);

#endif