#include "hive.hpp"
#include "bee.hpp"
#include "../utils/point_tree.hpp"
#include <iomanip>
#include <random>

void Hive::init(int totalbees) {
  tickoftheday = 0;
  this->totalbees = totalbees;
  this->ds = ds;
  this->activebees = 0;
  this->totalfood = 0;
}

// collecting the bees
Coordinates<double> Hive::move() {  
  RangeResult<double, Agent> *result = this->state.agents.range( pos, 0.1 );
  int size = result->size();

  for(int k = 0; k < size; k++){
    if( result->at(k).value->gettype() == AgentType::Bee ){
      std::shared_ptr<Agent> a = result->at(k).value;
      std::shared_ptr<Bee> b = std::dynamic_pointer_cast<Bee>(a);

      if( b->searching == false ){
        this->totalfood += b->food;
        
        PointValue<double, Agent> p = PointValue<double, Agent>(  result->at(k).value->getPosition(), a );
        this->state.agents.removeByPointValue( p );
      }
    }
  }

  return this->pos;
}

int Hive::getsize() { return ds->size(); }

// spawing new bees
void Hive::update() {
  tickoftheday += 1;
  double x = (double)tickoftheday / this->state.config.daylength;
  int release = totalbees * (-3 * (x - 0.5) * (x - 0.5) + 1);

  for (int k = 0; release > this->activebees; k++) {
    // release bees
    //  calc the percentage the bee is a scout
    int beespersource = 100;
    double scoutpercentage =
        ((double)totalbees - this->foodsources.size() * beespersource) /
        totalbees;
    double t = (double)(std::rand() % 10000) / 10000;

    if ((totalbees - this->foodsources.size() > 0 && t < scoutpercentage) ||
        this->foodsources.size() == 0) {

      std::random_device rd;
      std::mt19937 e2(rd());
      std::uniform_real_distribution<double> unif(0, 1);

      // Getting a random double value
      double randomX = unif(e2);
      double randomY = unif(e2);
      Coordinates<double> newBeePosition{randomX, randomY};

      std::shared_ptr<Bee> newBee =
          std::make_shared<Bee>(this->state, newBeePosition);

      newBee->init(Coordinates<double>{pos.x, pos.y},
                   Coordinates<double>{pos.x, pos.y}, true, false);

      PointValue<double, Agent> newPointValue(newBeePosition, newBee);
      this->state.agents.add(newPointValue);
    } else {

      Coordinates<double> newBeePosition{pos.x, pos.y};
      std::shared_ptr<Bee> newBee =
          std::make_shared<Bee>(this->state, newBeePosition);

      newBee->init(Coordinates<double>{pos.x, pos.y}, this->rand_fs(), true,
                   true);

      PointValue<double, Agent> newPointValue(newBeePosition, newBee);
      this->state.agents.add(newPointValue);
    }

    this->activebees += 1;
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