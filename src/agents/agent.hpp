#ifndef BEESIMULATION_AGENTS_AGENT_H
#define BEESIMULATION_AGENTS_AGENT_H

#include "../utils/point_tree.hpp"
#include <iostream>

class Agent {
public:
  // int x, y; -> if grid based, also int x,y

  // coordinates
  Coordinates<double> pos;

  // initialization of the Agent, function has to initialize all necessary
  // variables
  virtual void init();

  // movement of the agent
  virtual void move();

  // update of agent after move
  virtual void update();

  virtual void specialinteraction();

  // returns int of class (look enum declaration)
  virtual std::string gettype();

  // frees the allocated memory
  virtual void nuke();

  friend std::ostream &operator<<(std::ostream &output, Agent &a);
};

// returns the position of the agent 1 meter in one direction -> speed of agent
// needs still be implemented
Coordinates<double> getmovementvector(Coordinates<double> pos,
                                      Coordinates<double> target);

#endif