#include "agent.hpp"

void Agent::init() {
  std::cout << "This is an empty declaration! \n";
  return;
}

Coordinates<double> Agent::move() {
  std::cout << "This is an empty declaration! \n";
  return this->pos;
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

void Agent::setPosition(Coordinates<double> newPosition) {
  this->pos = newPosition;
}

void Agent::setState(WorldState *newState) { this->state = newState; }

Coordinates<double> getmovementvector(Coordinates<double> pos,
                                      Coordinates<double> target) {
  double x, y, pyt;
  x = target.x - pos.x;
  y = target.y - pos.y;
  pyt = sqrt(x * x + y * y);

  std::random_device rd;
  std::mt19937 e2(rd());
  std::uniform_real_distribution<double> unif(0, 0.001);

  // Getting a random double value
  double randomX = unif(e2);
  double randomY = unif(e2);

  x *= 5;
  y *= 5;

  if (pyt < 1) {
    return Coordinates<double>{pos.x + x + randomX, pos.y + y + randomY};
  } else {
    return Coordinates<double>{pos.x + x / pyt + randomX,
                               pos.y + y / pyt + randomY};
  }
}