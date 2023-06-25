#include "hive.hpp"
#include "bee.hpp"
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

void Hive::move() { return; }

int Hive::getsize() { return ds->size(); }

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
      // std::cout << "Scout: " << scoutpercentage << "  " << t << "\n";
      // scogut += 1;

      std::shared_ptr<Bee> newBee = std::make_shared<Bee>(this->state);

      std::random_device rd;
      std::mt19937 e2(rd());
      std::uniform_real_distribution<double> unif(0, 1);

      // Getting a random double value
      double randomX = unif(e2);
      double randomY = unif(e2);
      Coordinates<double> newBeePosition{randomX, randomY};
      std::cout << std::setprecision(15) << newBeePosition.x << "|"
                << newBeePosition.y << std::endl;

      newBee->init(newBeePosition, Coordinates<double>{pos.x, pos.y},
                   Coordinates<double>{pos.x, pos.y}, true, false, hstore);

      PointValue<double, Agent> newPointValue(newBeePosition, newBee);
      this->state.agents.add(newPointValue);
    } else {
      // std::cout << "Worker: " << scoutpercentage << "  " << t << "\n";
      // worker += 1;int scout = 0;
      int worker = 0;

      std::shared_ptr<Bee> newBee = std::make_shared<Bee>(this->state);
      Coordinates<double> newBeePosition{pos.x, pos.y};

      newBee->init(newBeePosition, Coordinates<double>{pos.x, pos.y},
                   hstore->rand_fs(), true, true, hstore);

      PointValue<double, Agent> newPointValue(newBeePosition, newBee);
      this->state.agents.add(newPointValue);
    }

    hstore->activebees += 1;
  }
  return;
}

std::string Hive::gettype() {
  std::string ret = "hive";
  return ret;
}

void Hive::nuke() {
  // removeagent = true;
  // delete hstore;
  // delete this;
}