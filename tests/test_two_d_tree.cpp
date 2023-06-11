#include <array>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

#include "../src/utils/two_d_tree.hpp"

TEST(TwoDTree, InitInt) {
  CoordinatesValue<int, int> testValues[] = {
      {{1, 2}, 5}, {{5, 3}, 6}, {{2, 2}, 1},
      {{8, 7}, 2}, {{9, 1}, 0}, {{1, 1}, 1},
  };

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  SUCCEED();
}

TEST(TwoDTree, InitDouble) {
  CoordinatesValue<double, double> testValues[] = {
      {{1.5, 2.2}, 5.1}, {{5.7, 3.8}, 6.2}, {{2.9, 2.8}, 1.7},
      {{8.3, 7.2}, 2.4}, {{9.5, 1.2}, 0},   {{1.3, 1.6}, 1.8},
  };

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  SUCCEED();
}

TEST(TwoDTree, SizeInt1) {
  CoordinatesValue<int, int> testValues[] = {
      {{1, 2}, 5}, {{5, 3}, 6}, {{2, 2}, 1},
      {{8, 7}, 2}, {{9, 1}, 0}, {{1, 1}, 1},
  };

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.size(), 6);
}

TEST(TwoDTree, SizeInt2) {
  CoordinatesValue<int, int> testValues[] = {
      {{1, 2}, 5}, {{5, 3}, 6}, {{2, 2}, 1}, {{8, 7}, 2}, {{9, 1}, 0},
      {{1, 1}, 1}, {{8, 7}, 2}, {{9, 1}, 0}, {{1, 1}, 1}, {{8, 7}, 2},
      {{9, 1}, 0}, {{1, 1}, 1}, {{8, 7}, 2}, {{9, 1}, 0}, {{1, 1}, 1},
      {{8, 7}, 2}, {{9, 1}, 0}, {{1, 1}, 1}, {{8, 7}, 2}, {{9, 1}, 0},
      {{1, 1}, 1}, {{8, 7}, 2}, {{9, 1}, 0}, {{1, 1}, 1}, {{8, 7}, 1}};

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.size(), 25);
}

TEST(TwoDTree, SizeDouble1) {
  CoordinatesValue<double, double> testValues[] = {
      {{1.5, 2.2}, 5.1}, {{5.7, 3.8}, 6.2}, {{2.9, 2.8}, 1.7},
      {{8.3, 7.2}, 2.4}, {{9.5, 1.2}, 0},   {{1.3, 1.6}, 1.8},
  };

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.size(), 6);
}

TEST(TwoDTree, SizeDouble2) {
  CoordinatesValue<double, double> testValues[] = {
      {{8.91, 9.71}, 2.41}, {{2.06, 2.38}, 0.31}, {{4.78, 0.13}, 1.12},
      {{6.90, 9.48}, 7.52}, {{1.45, 3.36}, 1.32}, {{6.62, 3.17}, 6.28},
      {{6.70, 1.11}, 2.59}, {{5.59, 3.43}, 6.52}, {{4.37, 1.89}, 0.48},
      {{6.27, 8.37}, 3.87}, {{8.87, 3.25}, 9.38}, {{7.28, 0.17}, 9.67},
      {{9.82, 9.92}, 1.61}, {{4.92, 5.54}, 0.88}, {{1.23, 0.62}, 1.97},
      {{4.09, 4.81}, 9.57}, {{3.65, 5.01}, 8.09}, {{4.65, 3.48}, 9.63},
      {{7.23, 0.11}, 3.13}, {{8.03, 4.38}, 0.70}, {{2.70, 4.71}, 8.09},
      {{9.67, 2.63}, 6.79}, {{4.90, 8.46}, 3.54}, {{2.04, 3.22}, 6.19},
      {{1.49, 6.52}, 6.69}, {{4.98, 4.31}, 6.33}, {{6.02, 4.05}, 5.50},
      {{8.32, 6.39}, 7.08}, {{2.31, 7.93}, 8.13}, {{9.88, 3.30}, 0.16},
      {{7.07, 9.53}, 4.53}, {{6.91, 8.12}, 8.81}, {{5.63, 6.07}, 0.69},
      {{7.08, 2.51}, 5.58}, {{1.19, 7.48}, 0.75}};

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.size(), 35);
}

TEST(TwoDTree, HeightInt1) {
  CoordinatesValue<int, int> testValues[] = {
      {{1, 2}, 5}, {{5, 3}, 6}, {{2, 2}, 1}, {{8, 7}, 2}, {{9, 1}, 0},
      {{1, 1}, 1}, {{8, 7}, 2}, {{9, 1}, 0}, {{1, 1}, 1}, {{8, 7}, 2},
      {{9, 1}, 0}, {{1, 1}, 1}, {{8, 7}, 2}, {{9, 1}, 0}, {{1, 1}, 1},
      {{8, 7}, 2}, {{9, 1}, 0}, {{1, 1}, 1}, {{8, 7}, 2}, {{9, 1}, 0},
      {{1, 1}, 1}, {{8, 7}, 2}, {{9, 1}, 0}, {{1, 1}, 1}, {{8, 7}, 1}};

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 5);
}

TEST(TwoDTree, HeightInt2) {
  CoordinatesValue<int, int> testValues[] = {
      {{1, 2}, 5}, {{5, 3}, 6}, {{2, 2}, 1}, {{8, 7}, 2}, {{9, 1}, 0},
      {{1, 1}, 1}, {{8, 7}, 2}, {{9, 1}, 0}, {{1, 1}, 1}, {{8, 7}, 2},
      {{1, 1}, 1}, {{8, 7}, 2}, {{9, 1}, 0}, {{1, 1}, 1}, {{8, 7}, 1}};

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 4);
}

TEST(TwoDTree, HeightDouble1) {
  CoordinatesValue<double, double> testValues[] = {
      {{1.5, 2.2}, 5.1}, {{5.7, 3.8}, 6.2}, {{2.9, 2.8}, 1.7},
      {{8.3, 7.2}, 2.4}, {{9.5, 1.2}, 0},   {{1.3, 1.6}, 1.8}};

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 3);
}

TEST(TwoDTree, HeightDouble2) {
  CoordinatesValue<double, double> testValues[] = {
      {{4.95, 0.04}, 1.05}, {{5.00, 0.55}, 9.68}, {{8.08, 2.43}, 3.64},
      {{6.73, 9.57}, 6.48}, {{3.07, 2.18}, 5.17}, {{6.43, 4.74}, 8.13},
      {{4.91, 1.74}, 0.63}, {{4.31, 3.18}, 6.13}, {{0.01, 3.02}, 1.26},
      {{8.13, 2.07}, 6.83}, {{7.80, 3.80}, 1.01}, {{0.91, 1.76}, 9.89},
      {{6.82, 5.28}, 4.96}, {{2.91, 4.91}, 5.60}, {{5.82, 8.85}, 5.16}};

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 4);
}

TEST(TwoDTree, DotRepresentationInt) {
  CoordinatesValue<int, int> testValues[] = {
      {{1, 6}, 5}, {{2, 5}, 6}, {{3, 4}, 1},
      {{4, 3}, 2}, {{6, 2}, 0}, {{6, 1}, 1},
  };

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  std::ofstream out("dottest_int.dot");
  out << tree.toDot();
  out.close();

  SUCCEED();
}

TEST(TwoDTree, DotRepresentationDouble) {
  CoordinatesValue<double, double> testValues[] = {
      {{8.91, 9.71}, 2.41}, {{2.06, 2.38}, 0.31}, {{4.78, 0.13}, 1.12},
      {{6.90, 9.48}, 7.52}, {{1.45, 3.36}, 1.32}, {{6.62, 3.17}, 6.28},
      {{6.70, 1.11}, 2.59}, {{5.59, 3.43}, 6.52}, {{4.37, 1.89}, 0.48},
      {{6.27, 8.37}, 3.87}, {{8.87, 3.25}, 9.38}, {{7.28, 0.17}, 9.67},
      {{9.82, 9.92}, 1.61}, {{4.92, 5.54}, 0.88}, {{1.23, 0.62}, 1.97},
      {{4.09, 4.81}, 9.57}, {{3.65, 5.01}, 8.09}, {{4.65, 3.48}, 9.63},
      {{7.23, 0.11}, 3.13}, {{8.03, 4.38}, 0.70}, {{2.70, 4.71}, 8.09},
      {{9.67, 2.63}, 6.79}, {{4.90, 8.46}, 3.54}, {{2.04, 3.22}, 6.19},
      {{1.49, 6.52}, 6.69}, {{4.98, 4.31}, 6.33}, {{6.02, 4.05}, 5.50},
      {{8.32, 6.39}, 7.08}, {{2.31, 7.93}, 8.13}, {{9.88, 3.30}, 0.16},
      {{7.07, 9.53}, 4.53}, {{6.91, 8.12}, 8.81}, {{5.63, 6.07}, 0.69},
      {{7.08, 2.51}, 5.58}, {{1.19, 7.48}, 0.75}};

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  std::ofstream out("dottest_double.dot");
  out << tree.toDot();
  out.close();

  SUCCEED();
}
