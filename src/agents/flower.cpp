#include "flower.hpp"

void Flower::init(double bloomlength, double maxproduction, int peak) {
  this->bloomlength = bloomlength;
  this->maxproduction = maxproduction;
  this->peak = peak;
  this->size = 0;
}

void Flower::move() { return; }

void Flower::update() {
  int storesize = size;
  size += maxproduction / (pow(bloomlength, abs(day - peak)));

  // std::cout << day << "\n";
  // std::cout << (int) (maxproduction / ( pow(bloomlength, abs(day-peak))) )
  // << "\n";

  if ((int)(maxproduction / (pow(bloomlength, abs(day - peak)))) == 0 &&
      day > peak) {
    nuke();
    return;
  }
}

std::string Flower::gettype() {
  std::string ret = "flower";
  return ret;
}

void Flower::nuke() {
  // std::cout << "removed flower\n";
  removeagent = true;
  delete this;
}