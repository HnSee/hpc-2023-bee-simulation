#include "bee.hpp"
#include "flower.hpp"
#include "src/agents/agent.hpp"

void Bee::init(Coordinates<double> hivepos, Coordinates<double> destination,
               bool searching, bool worker, Coordinates<double> posi) {
  this->hivepos = hivepos;
  this->destination = destination;
  this->searching = searching;
  this->worker = worker;
  this->food = 0;
  this->pos = posi;
  this->found = false;
  //std::cout << pos.x << "   " << pos.y << "\n";
}

Coordinates<double> Bee::move(ChunkBounds worldBounds) {
  if (worker) {
    if (searching){
      pos = getmovementvector(pos, destination);

      if (pos.x < destination.x + 0.1 && pos.x > destination.x - 0.1 &&
          pos.y < destination.y + 0.1 && pos.y > destination.y - 0.1) {
        // has to search
        food = 1;
        searching = false;
      }
    } else {
      pos = getmovementvector(pos, hivepos);
    }
  } else {
    if (searching) {
      
      //std::cout << pos.x << "  " << pos.y << "\n";
      pos = getmovementvector(pos, destination);
      //std::cout << pos.x << "  " << pos.y << "\n";
      //std::cout << "\n";

      // check if food is near
      // if food is near, store position in destination
      auto result = this->state->agents.range(pos, 0.1);
      int size = result->size();

      for (int k = 0; k < size; k++) {
        if (result->at(k).value->gettype() == AgentType::Flower) {
          std::shared_ptr<Agent> a = result->at(k).value;
          std::shared_ptr<Flower> f = std::dynamic_pointer_cast<Flower>(a);
          this->searching = false;
          this->destination = result->at(k).value->pos;
          this->found = true;
          std::cout << "SHOULD BE COLLECTED IN THE FUTURE!!!\n\n\n\n\n";
        }
      }

      // if scout reaces destination and doesnt find anything then the scout
      // either generates next location or returns
      if (pos.x < destination.x + 1 && pos.x > destination.x - 1 &&
          pos.y < destination.y + 1 && pos.y > destination.y - 1) {
        // std::cout << "new destination!" << "\n";
        if (this->state->config.scoutindurance > std::rand() % 101) {
          // std::cout << "Destination: " << destination.x << "\n";
          destination.x += (std::rand() % 400) - 200;
          destination.y += (std::rand() % 400) - 200;
          // std::cout << "Destination: " << destination.x << "\n";
        } else {
          searching = false;
        }
      }
    } else {
      pos = getmovementvector(pos, hivepos);
    }
  }

  pos.clamp(worldBounds.xMin, worldBounds.xMax, worldBounds.yMin,
            worldBounds.yMax);


  //std::cout << pos.x << "    " << pos.y << "\n";
  return pos;
}

void Bee::update() { return; }

Bee_struct Bee::get_struct() {
  Bee_struct bs = {this->searching,  this->worker, this->activebbes,
                   this->foodstore,  this->food,   this->hivepos,
                   this->destination};
}

AgentType Bee::gettype() const { return AgentType::Bee; }