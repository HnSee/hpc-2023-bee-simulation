#ifndef BEESIMULATION_TESTS_MATCHERS_H
#define BEESIMULATION_TESTS_MATCHERS_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

MATCHER_P3(EqRangeResult, x, y, v, "") {
  return arg.point.x == x && arg.point.y == y && *(arg.value) == v;
}

#endif