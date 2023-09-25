#include "agent.hpp"
#include <cmath>

void Agent::init() {
  std::cout << "This is an empty declaration! \n";
  return;
}

Coordinates<double> Agent::move(ChunkBounds worldBounds) {
  std::cout << "This is an empty declaration! \n";
  return this->pos;
}

// update of agent after move
void Agent::update() {
  std::cout << "This is an empty declaration! \n";
  return;
}

std::ostream &operator<<(std::ostream &output, const Agent &a) {
  output << "[" << a.gettype() << "]";
  return output;
}

Coordinates<double> Agent::getPosition() const { return this->pos; }

void Agent::setPosition(Coordinates<double> newPosition) {
  this->pos = newPosition;
}

void Agent::setState(WorldState *newState) { this->state = newState; }

Coordinates<double> getmovementvector(Coordinates<double> posi,
                                      Coordinates<double> target) {
  double x;
  double y;
  double pyt;

  x = target.x - posi.x;
  y = target.y - posi.y;
  pyt = sqrt((x * x) + (y * y));

  std::random_device rd;
  std::mt19937 e2(rd());
  std::uniform_real_distribution<double> unif(0, 0.00001);

  // Getting a random double value
  double randomX = unif(e2);
  double randomY = unif(e2);

  if (pyt < 1) {
    return Coordinates<double>{posi.x + x + randomX, posi.y + y + randomY};
  } else {
    return Coordinates<double>{posi.x + (+x) / (pyt + 1) + randomX,
                               posi.y + (+y) / (pyt + 1) + randomY};
  }
}
