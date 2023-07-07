#include "flower.hpp"

void Flower::init(double bloomlength, double maxproduction, int peak) {
  this->bloomlength = bloomlength;
  this->maxproduction = maxproduction;
  this->peak = peak;
  this->size = 0;
}

Coordinates<double> Flower::move(ChunkBounds worldBounds) { return this->pos; }

void Flower::update() {
  int storesize = size;
  size += maxproduction / (pow(bloomlength, abs(this->state->day - peak)));

  // std::cout << day << "\n";
  // std::cout << (int) (maxproduction / ( pow(bloomlength, abs(day-peak))) )
  // << "\n";

  if ((int)(maxproduction / (pow(bloomlength, abs(this->state->day - peak)))) ==
          0 &&
      this->state->day > peak) {
    return;
  }
}

AgentType Flower::gettype() const { return AgentType::Flower; }