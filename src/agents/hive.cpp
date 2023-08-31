#include "hive.hpp"
#include "../utils/point_tree.hpp"
#include "bee.hpp"
#include <iomanip>
#include <random>

void Hive::init(int totalbees) {
  tickoftheday = 0;
  this->totalbees = 40000;
  this->ds = ds;
  this->activebees = 0;
  this->totalfood = 0;
}

Coordinates<double> Hive::move(ChunkBounds worldBounds) { return this->pos; }

int Hive::getsize() const { return ds->size(); }

// spawing new bees
void Hive::update() {
  tickoftheday += 1;
  double x = (double)tickoftheday / this->state->config.daylength;
  int release = totalbees * (-3.5 * (x - 0.5) * (x - 0.5) + 1);

  for (int k = 0; release > this->activebees; k++) {
    // release bees
    //  calc the percentage the bee is a scout
    int beespersource = 200; // this->state->config.beespersource;
    double scoutpercentage =
        ((double)totalbees - this->foodsources.size() * beespersource) /
        totalbees;

    // calculate the new position of the bee
    double t = (double)(std::rand() % 10000) / 10000;
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<double> unif(0, 10);
    // Getting a random double value
    double randomX = unif(e2);
    double randomY = unif(e2);

    Coordinates<double> newBeePosition{randomX + this->pos.x,
                                       this->pos.y + randomY};

    if ((totalbees - this->foodsources.size() > 0 && t < scoutpercentage) ||
        this->foodsources.size() == 0) {

      std::shared_ptr<Bee> newBee =
          std::make_shared<Bee>(this->state, newBeePosition);

      newBee->init(Coordinates<double>{newBeePosition.x, newBeePosition.y},
                   Coordinates<double>{newBeePosition.x, newBeePosition.y},
                   true, false,
                   Coordinates<double>{newBeePosition.x, newBeePosition.y});

      PointValue<double, Agent> newPointValue(newBeePosition, newBee);
      this->state->agents.add(newPointValue);
    } else {
      Coordinates<double> newBeePosition{pos.x, pos.y};
      std::shared_ptr<Bee> newBee =
          std::make_shared<Bee>(this->state, newBeePosition);

      newBee->init(Coordinates<double>{pos.x, pos.y}, this->rand_fs(), true,
                   true,
                   Coordinates<double>{newBeePosition.x, newBeePosition.y});

      PointValue<double, Agent> newPointValue(newBeePosition, newBee);
      this->state->agents.add(newPointValue);
    }

    this->activebees += 1;
  }

  auto result = this->state->agents.range(pos, 0.01);
  int size = result->size();

  for (int k = 0; k < size; k++) {
    if (result->at(k).value->gettype() == AgentType::Bee) {

      std::shared_ptr<Agent> a = result->at(k).value;
      std::shared_ptr<Bee> b = std::dynamic_pointer_cast<Bee>(a);

      if (b->searching == false && b->worker) {
        this->totalfood += b->food;

        PointValue<double, Agent> p =
            PointValue<double, Agent>(result->at(k).value->getPosition(), a);
        this->state->agents.removeByPointValue(p);
        this->activebees -= 1;
      }

      if (b->searching == false && !b->worker && b->found) {
        this->add_fs(b->destination);

        PointValue<double, Agent> p =
            PointValue<double, Agent>(result->at(k).value->getPosition(), a);
        this->state->agents.removeByPointValue(p);
        this->activebees -= 1;
      }
    }
  }

  return;
}

void Hive::add_fs(Coordinates<double> p) {
  for (int k = foodsources.size() - 1; k > 0; k--) {
    if (foodsources.at(k).x == p.x && foodsources.at(k).y == p.y) {
      return;
    }
  }
  foodsources.push_back(p);
}

void Hive::rem_fs(Coordinates<double> p) {
  for (int k = 0; k < foodsources.size(); k++) {
    if (foodsources.at(k).x == p.x && foodsources.at(k).y == p.y) {
      foodsources.at(k) = foodsources.at(foodsources.size() - 1);
      foodsources.pop_back();
      return;
    }
  }
}

Coordinates<double> Hive::rand_fs() {
  if (foodsources.size() == 0) {
    throw std::invalid_argument("must be at least one argument");
  }

  int r = std::rand() % foodsources.size();

  return foodsources.at(r);
}

AgentType Hive::gettype() const { return AgentType::Hive; }