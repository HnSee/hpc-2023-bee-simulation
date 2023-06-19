#include "agent.hpp"

void Agent::init() {
  std::cout << "This is an empty declaration! \n";
  return;
}

void Agent::move() {
  std::cout << "This is an empty declaration! \n";
  return;
};

// update of agent after move
void Agent::update() {
  std::cout << "This is an empty declaration! \n";
  return;
};

void Agent::specialinteraction() {
  std::cout << "This is an empty declaration! \n";
  return;
};

// returns int of class (look enum declaration)
std::string Agent::gettype() {
  std::cout << "This is an empty declaration! \n";
  return NULL;
};

// frees the allocated memory
void Agent::nuke() { std::cout << "This is an empty declaration! \n"; }

std::ostream &operator<<(std::ostream &output, Agent &a) {
  output << "[" << a.gettype() << "]";
  return output;
}

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