#include "bee.hpp"
#include "flower.hpp"

void Bee::init(Coordinates<double> hive, Coordinates<double> destination,
               bool searching, bool worker, HiveBeeAccess *hstore) {
  this->hivepos = hivepos;
  this->destination = destination;
  this->searching = searching;
  this->worker = worker;
  this->hstore = hstore;
  hstore->foodstore = 0;
}

Coordinates<double> Bee::move() {
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
      }
    }
  } else {
    if (searching) {
      pos = getmovementvector(pos, destination);
      // check if food is near
      // if food is near, store position in destination,

      RangeResult<double, Agent> *result = this->state.agents.range(pos, 0.1);
      int size = result->size();

      for (int k = 0; k < size; k++) {
        if (result->at(k).value->gettype() == AgentType::Flower) {
          std::shared_ptr<Agent> a = result->at(k).value;
          std::shared_ptr<Flower> f = std::dynamic_pointer_cast<Flower>(a);

          if (f->size > 100) {
            this->food += 10;
            f->size -= 10;
          }
        }
      }

      this->searching = false;

      // if scout reaches destination and doesnt find anything then the scout
      // either generates next location or returns
      if (pos.x == destination.x && pos.y == destination.y) {
        if (this->state.config.scoutindurance > std::rand() % 101) {
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
      }
    }
  }

  return this->pos;
}

void Bee::update() { return; }

AgentType Bee::gettype() const { return AgentType::Bee; }