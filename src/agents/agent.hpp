#ifndef BEESIMULATION_AGENTS_AGENT_H
#define BEESIMULATION_AGENTS_AGENT_H

#include "../utils/point_tree.hpp"
#include "agent_type.hpp"
#include <iostream>

class WorldState;

class Agent {
public:
  Agent(WorldState &state, Coordinates<double> pos) : state(state), pos(pos) {}

  // initialization of the Agent, function has to initialize all necessary
  // variables
  virtual void init();

  // movement of the agent
  void move();

  // update of agent after move
  virtual void update();

  virtual void specialinteraction();

  // returns int of class (look enum declaration)
  virtual AgentType gettype() const = 0;

  // frees the allocated memory
  virtual void nuke();

  Coordinates<double> getPosition();

  friend std::ostream &operator<<(std::ostream &output, const Agent &a);

  WorldState &state;

protected:
  // coordinates
  Coordinates<double> pos;
};

// returns the position of the agent 1 meter in one direction -> speed of agent
// needs still be implemented
Coordinates<double> getmovementvector(Coordinates<double> pos,
                                      Coordinates<double> target);

#endif