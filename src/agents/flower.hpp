#ifndef BEESIMULATION_AGENTS_FLOWER_H
#define BEESIMULATION_AGENTS_FLOWER_H

#include "../world/worldstate.hpp"
#include "agent.hpp"
#include "hive_bee_access.hpp"

class Flower : public Agent {
public:
  using Agent::Agent;

  // how long a flower produces food closer to one -> longer blooming ( 1=
  // blooming forever )
  double bloomlength;

  // how much a flower produces at maximum -> larger = more production
  double maxproduction;

  // day of the max production
  int peak;

  // regenerates with ((maxproduction)/(bloomlength^(abs(x-peak))))
  double size;

  void init(double bloomlength, double maxproduction, int peak);

  void move();

  void update();

  std::string gettype();

  void nuke();
};

#endif