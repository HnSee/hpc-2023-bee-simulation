#include "hive.hpp"
#include "bee.hpp"
#include "../utils/point_tree.hpp"
#include <iomanip>
#include <random>


void Hive::init(int totalbees, HiveBeeAccess *hstore) {
  tickoftheday = 0;
  this->totalbees = totalbees;
  this->ds = ds;
  this->hstore = hstore;
  this->hstore->activebees = 0;
  this->hstore->foodstore = 0;
}

// collecting the bees
void Hive::move() {  
  this->state.agents.range( pos, 0.1 );
  
  /*
  RangeResult<double, Agent> *result = 
  int size = result->size();

  for(int k = 0; k < size; k++){
    if( result->at(k).value->gettype() == AgentType::Bee ){

      //std::shared_ptr<Agent> a = result->at(k).value;
      //PointValue<double, Agent> p = PointValue<double, Agent>(  result->at(k).value->pos, a );
      //this->state.agents.removeByPointValue( p );
    }
    //
  }
  */

  return;
}

int Hive::getsize() { return ds->size(); }

// spawing new bees
void Hive::update() {
  tickoftheday += 1;
  double x = (double)tickoftheday / this->state.config.daylength;
  int release = totalbees * (-3 * (x - 0.5) * (x - 0.5) + 1);

  for (int k = 0; release > hstore->activebees; k++) {
    // release bees
    //  calc the percentage the bee is a scout
    int beespersource = 100;
    double scoutpercentage =
        ((double)totalbees - hstore->foodsources.size() * beespersource) /
        totalbees;
    double t = (double)(std::rand() % 10000) / 10000;

    if ((totalbees - hstore->foodsources.size() > 0 && t < scoutpercentage) ||
        hstore->foodsources.size() == 0) {

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
                   Coordinates<double>{pos.x, pos.y}, true, false, hstore);

      PointValue<double, Agent> newPointValue(newBeePosition, newBee);
      this->state.agents.add(newPointValue);
    } else {

      Coordinates<double> newBeePosition{pos.x, pos.y};
      std::shared_ptr<Bee> newBee =
          std::make_shared<Bee>(this->state, newBeePosition);

      newBee->init(Coordinates<double>{pos.x, pos.y}, hstore->rand_fs(), true,
                   true, hstore);

      PointValue<double, Agent> newPointValue(newBeePosition, newBee);
      this->state.agents.add(newPointValue);
    }

    hstore->activebees += 1;
  }
  return;
}

AgentType Hive::gettype() const { return AgentType::Hive; }