#include "agent.hpp"

void Agent::init() {
  std::cout << "This is an empty declaration! \n";
  return;
}

void Agent::move() {
  std::cout << "This is an empty declaration! \n";
  return;
}

// update of agent after move
void Agent::update() {
  std::cout << "This is an empty declaration! \n";
  return;
}

void Agent::specialinteraction() {
  std::cout << "This is an empty declaration! \n";
  return;
}

std::ostream &operator<<(std::ostream &output, const Agent &a) {
  output << "[" << a.gettype() << "]";
  return output;
}

Coordinates<double> Agent::getPosition() { return this->pos; }

Coordinates<double> getmovementvector(Coordinates<double> pos,
                                      Coordinates<double> target) {
  double x, y, pyt;
  x = target.x - pos.x;
  y = target.y - pos.y;
  pyt = sqrt(x * x + y * y);

  if (pyt < 1) {
    return Coordinates<double>{pos.x + x, pos.y + y};
  } else {
    return Coordinates<double>{pos.x + x / pyt, pos.y + y / pyt};
  }
}