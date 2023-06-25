#include "worldstate.hpp"

void WorldState::tick() {
  this->agents.traverse(
      [](const PointValue<double, Agent> &pv) { pv.value->update(); });
}
