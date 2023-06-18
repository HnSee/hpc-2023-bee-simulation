#include "hive.hpp"
#include "bee.hpp"

void Hive::init(int totalbees, std::vector<Agent *> *ds,
                HiveBeeAccess *hstore) {
  tickoftheday = 0;
  this->totalbees = totalbees;
  this->ds = ds;
  this->hstore = hstore;
  this->hstore->activebees = 0;
  this->hstore->foodstore = 0;
}

void Hive::move() {
  tickoftheday += 1;
  return;
}

int Hive::getsize() { return ds->size(); }

void Hive::update() {
  double x = (double)tickoftheday / daylength;
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
      scout += 1;
      Bee *b = new Bee;
      b->init(Coordinates<double>{pos.x, pos.y},
              Coordinates<double>{pos.x, pos.y},
              Coordinates<double>{pos.x, pos.y}, true, false, hstore);
      ds->push_back(b);
    } else {
      // std::cout << "Worker: " << scoutpercentage << "  " << t << "\n";
      worker += 1;
      Bee *b = new Bee;
      b->init(Coordinates<double>{pos.x, pos.y},
              Coordinates<double>{pos.x, pos.y}, hstore->rand_fs(), true, true,
              hstore);
      ds->push_back(b);
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
  removeagent = true;
  delete hstore;
  delete this;
}