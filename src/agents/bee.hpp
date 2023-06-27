#ifndef BEESIMULATION_AGENTS_BEE_H
#define BEESIMULATION_AGENTS_BEE_H

#include "../world/worldstate.hpp"
#include "agent.hpp"
#include "hive_bee_access.hpp"

class Bee : public Agent {
public:
  using Agent::Agent;

  // hiveptr
  HiveBeeAccess *hstore;

  // wether the bee is flying to the destination or the hive
  bool searching;

  // whether or not the bee is a scout
  bool worker;

  // food the bee is carrying
  int food;

  // position of the hive
  Coordinates<double> hivepos;

  // position send to
  Coordinates<double> destination;

  void init(Coordinates<double> hive, Coordinates<double> destination,
            bool searching, bool worker, HiveBeeAccess *hstore);

  void move();

  void update();

  AgentType gettype() const;
};

#endif