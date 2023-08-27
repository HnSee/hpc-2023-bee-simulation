#ifndef BEESIMULATION_AGENTS_BEE_H
#define BEESIMULATION_AGENTS_BEE_H

#include "../world/worldstate.hpp"
#include "agent.hpp"

class Bee : public Agent {
public:
  using Agent::Agent;

  // wether the bee is flying to the destination or the hive
  bool searching;

  // did the bee find food
  bool found;

  // whether or not the bee is a scout
  bool worker;

  int activebbes;

  int foodstore;

  // food the bee is carrying
  int food;

  // position of the hive
  Coordinates<double> hivepos;

  // position send to
  Coordinates<double> destination;

  void init(Coordinates<double> hive, Coordinates<double> destination,
            bool searching, bool worker, Coordinates<double> posi);

  Coordinates<double> move(ChunkBounds worldBounds) override;

  void update() override;

  AgentType gettype() const override;
};

#endif