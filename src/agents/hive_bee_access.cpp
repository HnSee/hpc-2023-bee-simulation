#include "hive_bee_access.hpp"

int scout = 0;
int worker = 0;

void HiveBeeAccess::addfood(int x) { foodstore = foodstore + x; }

// adds foodsource
void HiveBeeAccess::add_fs(Coordinates<double> p) {
  for (int k = foodsources.size() - 1; k > 0; k--) {
    if (foodsources.at(k).x == p.x && foodsources.at(k).y == p.y) {
      return;
    }
  }
  foodsources.push_back(p);
}

// removes foodsource
void HiveBeeAccess::rem_fs(Coordinates<double> p) {
  for (int k = 0; k < foodsources.size(); k++) {
    if (foodsources.at(k).x == p.x && foodsources.at(k).y == p.y) {
      foodsources.at(k) = foodsources.at(foodsources.size() - 1);
      foodsources.pop_back();
      return;
    }
  }
}

// get random foodsource
Coordinates<double> HiveBeeAccess::rand_fs() {
  if (foodsources.size() == 0) {
    throw std::invalid_argument("must be at least one argument");
  }

  int r = std::rand() % foodsources.size();

  return foodsources.at(r);
}

bool check_agent_removal(int j) {
  if (removeagent == true) {
    agents[j] = agents[agents.size() - 1];
    agents.pop_back();
    removeagent = false;
    return true;
  }
  return false;
}

// one move
void tick(int j) {
  agents[j]->move();

  if (check_agent_removal(j)) {
    return;
  }

  agents[j]->update();

  if (check_agent_removal(j)) {
    return;
  }
}