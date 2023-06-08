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

    std::pair<Coordinate, T *> newXIndex =
        std::make_pair(c.first, &(*result).second);
    std::pair<Coordinate, T *> newYIndex =
        std::make_pair(c.second, &(*result).second);

    this->xIndex.insert(newXIndex);
    this->yIndex.insert(newYIndex);
  }

  bool remove(Coordinates c, T v) {
    auto coordinateEntities = this->data.equal_range(c);
    auto it = coordinateEntities.first;

    bool foundInData = false;
    bool foundInXIndex = false;
    bool foundInYIndex = false;

    for (; it != coordinateEntities.second; ++it) {
      if (it->second == v) {
        this->data.erase(it);
        foundInData = true;

        auto xIndexCoordinates = this->xIndex.equal_range(c.first);
        auto xIt = xIndexCoordinates.first;
        for (; xIt != xIndexCoordinates.second; ++xIt) {
          if (xIt->second == &(it->second)) {
            this->xIndex.erase(xIt);
            foundInXIndex = true;
            break;
          }
        }

        auto yIndexCoordinates = this->yIndex.equal_range(c.second);
        auto yIt = yIndexCoordinates.first;
        for (; yIt != yIndexCoordinates.second; ++yIt) {
          if (yIt->second == &(it->second)) {
            this->yIndex.erase(yIt);
            foundInYIndex = true;
            break;
          }
        }

        break;
      }
    }

    return foundInData && foundInXIndex && foundInYIndex;
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