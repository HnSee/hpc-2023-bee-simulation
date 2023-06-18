#include "bee.hpp"

void Bee::init(Coordinates<double> pos, Coordinates<double> hive,
               Coordinates<double> destination, bool searching, bool worker,
               HiveBeeAccess *hstore) {
  this->pos = pos;
  this->hivepos = hivepos;
  this->destination = destination;
  this->searching = searching;
  this->worker = worker;
  this->hstore = hstore;
  hstore->foodstore = 0;
}

void Bee::move() {
  if (worker) {
    if (searching) {
      pos = getmovementvector(pos, destination);
      if (pos.x == destination.x && pos.y == destination.y) {
        food = 1;
        searching = false;
      }
    } else {
      pos = getmovementvector(pos, hivepos);

      if (pos.x == hivepos.x && pos.y == hivepos.y) {
        hstore->addfood(food);
        nuke();
      }
    }
  } else {
    if (searching) {
      pos = getmovementvector(pos, destination);
      // check if food is near
      // if food is near, store position in destination,
      // if scout reaches destination and doesnt find anything then the scout
      // either generates next location or returns
      if (pos.x == destination.x && pos.y == destination.y) {
        if (scoutindurance > std::rand() % 101) {
          destination.x += (std::rand() % 2000) - 1000;
          destination.y += (std::rand() % 2000) - 1000;
        } else {
          searching = false;
        }
      }
    } else {
      pos = getmovementvector(pos, hivepos);
      if (pos.x == hivepos.x && pos.y == hivepos.y) {
        hstore->add_fs(destination);
        nuke();
      }
    }
  }
}

void Bee::update() { return; }

std::string Bee::gettype() {
  std::string ret = "bee";
  return ret;
}

void Bee::nuke() {
  // std::cout << "removed bee\n";
  hstore->activebees -= 1;
  removeagent = true;
  delete this;
}