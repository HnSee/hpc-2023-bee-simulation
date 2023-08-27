#ifndef BEESIMULATION_UTILS_COORDINATES_H
#define BEESIMULATION_UTILS_COORDINATES_H

#include <cmath>
#include <fmt/core.h>

const double DOUBLE_EPSILON = 0.0000001;

enum class Axis { X, Y };

template <typename C> struct Coordinates {
  C x;
  C y;

  bool equals(const Coordinates<C> &c) {
    return this->x == c.x && this->y == c.y;
  }

  friend bool operator==(const Coordinates<C> &l, const Coordinates<C> &r) {
    return l.x == r.x && l.y == r.y;
  }

  friend bool operator!=(const Coordinates<C> &l, const Coordinates<C> &r) {
    return l.x != r.x || l.y != r.y;
  }

  bool smallerThan(const Coordinates<C> &c, Axis axis) const {
    return axis == Axis::X ? this->x < c.x : this->y < c.y;
  }

  double difference(const Coordinates<C> &c, Axis axis) const {
    return axis == Axis::X ? this->x - c.x : this->y - c.y;
  }

  double getDistanceTo(const Coordinates<C> &c) const {
    return calculateEuclideanDistance(*this, c);
  }

  void clamp(C xMin, C xMax, C yMin, C yMax) {
    this->x = std::max(xMin, std::min(this->x, xMax));
    this->y = std::max(yMin, std::min(this->y, yMax));
  }

  static double calculateEuclideanDistance(const Coordinates<C> &point1,
                                           const Coordinates<C> &point2) {
    return std::sqrt(std::pow(point2.x - point1.x, 2) +
                     std::pow(point2.y - point1.y, 2));
  }
};

template <typename C>
struct fmt::formatter<Coordinates<C>> : fmt::formatter<std::string> {
  auto format(const Coordinates<C> &c, format_context &ctx) const
      -> format_context::iterator {
    return fmt::format_to(ctx.out(), "({}|{})", c.x, c.y);
  }
};

#endif