#ifndef BEESIMULATION_UTILS_COORDINATE_MATRIX_H
#define BEESIMULATION_UTILS_COORDINATE_MATRIX_H

#include "pair_hash.hpp"
#include <iterator>
#include <map>
#include <unordered_map>
#include <unordered_set>

typedef double Coordinate;
typedef std::pair<Coordinate, Coordinate> Coordinates;

template <typename T> class CoordinateMatrix {
  using TCoordinateIterator = typename std::multimap<Coordinate, T>::iterator;
  using TDataIterator =
      typename std::unordered_multimap<Coordinates, T, PairHash>::iterator;

public:
  CoordinateMatrix(std::size_t size) { this->data.reserve(size); }

  CoordinateMatrix() : CoordinateMatrix(10) {}

  std::pair<TDataIterator, TDataIterator> get(Coordinates &c) {
    return this->data.equal_range(c);
  }

  void set(Coordinates c, T v) {
    TDataIterator result = this->data.insert(std::make_pair(c, v));

    std::pair<Coordinate, T *> newXIndex = std::make_pair(c.first, &(*result).second);
    std::pair<Coordinate, T *> newYIndex = std::make_pair(c.second, &(*result).second);

    this->xIndex.insert(newXIndex);
    this->yIndex.insert(newYIndex);
  }

  void remove(Coordinates c, T v) {
    // TODO
    for (auto it = this->data.find(c); it != this->data.end(); it++) {
    }
  }

  std::pair<TCoordinateIterator, TCoordinateIterator> getByX(Coordinate x) {
    return this->xIndex.equal_range(x);
  }

  std::pair<TCoordinateIterator, TCoordinateIterator> getByY(Coordinate y) {
    return this->yIndex.equal_range(y);
  }

private:
  std::unordered_multimap<Coordinates, T, PairHash> data;
  std::multimap<Coordinate, T *> xIndex;
  std::multimap<Coordinate, T *> yIndex;
};

#endif