#include <array>
#include <cmath>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../src/utils/two_d_tree.hpp"

template <typename C, typename V> CoordinatesValue<C, V> cv(C x, C y, V v) {
  return CoordinatesValue<C, V>(x, y, v);
}

TEST(TwoDTree, InitInt) {
  CoordinatesValue<int, int> testValues[] = {cv(0, 1, 2), cv(5, 3, 6),
                                             cv(2, 2, 1), cv(8, 7, 2),
                                             cv(9, 1, 0), cv(1, 1, 1)};

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  SUCCEED();
}

TEST(TwoDTree, InitDouble) {
  CoordinatesValue<double, double> testValues[] = {
      cv(1.5, 2.2, 5.1), cv(5.7, 3.8, 6.2), cv(2.9, 2.8, 1.7),
      cv(8.3, 7.2, 2.4), cv(9.5, 1.2, 0.),  cv(1.3, 1.6, 1.8),
  };

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  SUCCEED();
}

TEST(TwoDTree, SizeInt1) {
  CoordinatesValue<int, int> testValues[] = {
      cv(1, 2, 5), cv(5, 3, 6), cv(2, 2, 1),
      cv(8, 7, 2), cv(9, 1, 0), cv(1, 1, 1),
  };

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.size(), 6);
}

TEST(TwoDTree, SizeInt2) {
  CoordinatesValue<int, int> testValues[] = {
      cv(1, 2, 5), cv(5, 3, 6), cv(2, 2, 1), cv(8, 7, 2), cv(9, 1, 0),
      cv(1, 1, 1), cv(8, 7, 2), cv(9, 1, 0), cv(1, 1, 1), cv(8, 7, 2),
      cv(9, 1, 0), cv(1, 1, 1), cv(8, 7, 2), cv(9, 1, 0), cv(1, 1, 1),
      cv(8, 7, 2), cv(9, 1, 0), cv(1, 1, 1), cv(8, 7, 2), cv(9, 1, 0),
      cv(1, 1, 1), cv(8, 7, 2), cv(9, 1, 0), cv(1, 1, 1), cv(8, 7, 1)};

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.size(), 25);
}

TEST(TwoDTree, SizeDouble1) {
  CoordinatesValue<double, double> testValues[] = {
      cv(1.5, 2.2, 5.1), cv(5.7, 3.8, 6.2), cv(2.9, 2.8, 1.7),
      cv(8.3, 7.2, 2.4), cv(9.5, 1.2, 0.),  cv(1.3, 1.6, 1.8),
  };

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.size(), 6);
}

TEST(TwoDTree, SizeDouble2) {
  CoordinatesValue<double, double> testValues[] = {
      cv(8.91, 9.71, 2.41), cv(2.06, 2.38, 0.31), cv(4.78, 0.13, 1.12),
      cv(6.90, 9.48, 7.52), cv(1.45, 3.36, 1.32), cv(6.62, 3.17, 6.28),
      cv(6.70, 1.11, 2.59), cv(5.59, 3.43, 6.52), cv(4.37, 1.89, 0.48),
      cv(6.27, 8.37, 3.87), cv(8.87, 3.25, 9.38), cv(7.28, 0.17, 9.67),
      cv(9.82, 9.92, 1.61), cv(4.92, 5.54, 0.88), cv(1.23, 0.62, 1.97),
      cv(4.09, 4.81, 9.57), cv(3.65, 5.01, 8.09), cv(4.65, 3.48, 9.63),
      cv(7.23, 0.11, 3.13), cv(8.03, 4.38, 0.70), cv(2.70, 4.71, 8.09),
      cv(9.67, 2.63, 6.79), cv(4.90, 8.46, 3.54), cv(2.04, 3.22, 6.19),
      cv(1.49, 6.52, 6.69), cv(4.98, 4.31, 6.33), cv(6.02, 4.05, 5.50),
      cv(8.32, 6.39, 7.08), cv(2.31, 7.93, 8.13), cv(9.88, 3.30, 0.16),
      cv(7.07, 9.53, 4.53), cv(6.91, 8.12, 8.81), cv(5.63, 6.07, 0.69),
      cv(7.08, 2.51, 5.58), cv(1.19, 7.48, 0.75)};

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.size(), 35);
}

TEST(TwoDTree, HeightInt1) {
  CoordinatesValue<int, int> testValues[] = {
      cv(1, 2, 5), cv(5, 3, 6), cv(2, 2, 1), cv(8, 7, 2), cv(9, 1, 0),
      cv(1, 1, 1), cv(8, 7, 2), cv(9, 1, 0), cv(1, 1, 1), cv(8, 7, 2),
      cv(9, 1, 0), cv(1, 1, 1), cv(8, 7, 2), cv(9, 1, 0), cv(1, 1, 1),
      cv(8, 7, 2), cv(9, 1, 0), cv(1, 1, 1), cv(8, 7, 2), cv(9, 1, 0),
      cv(1, 1, 1), cv(8, 7, 2), cv(9, 1, 0), cv(1, 1, 1), cv(8, 7, 1)};

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 5);
}

TEST(TwoDTree, HeightInt2) {
  CoordinatesValue<int, int> testValues[] = {
      cv(1, 2, 5), cv(5, 3, 6), cv(2, 2, 1), cv(8, 7, 2), cv(9, 1, 0),
      cv(1, 1, 1), cv(8, 7, 2), cv(9, 1, 0), cv(1, 1, 1), cv(8, 7, 2),
      cv(1, 1, 1), cv(8, 7, 2), cv(9, 1, 0), cv(1, 1, 1), cv(8, 7, 1)};

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 4);
}

TEST(TwoDTree, HeightDouble1) {
  CoordinatesValue<double, double> testValues[] = {
      cv(1.5, 2.2, 5.1), cv(5.7, 3.8, 6.2), cv(2.9, 2.8, 1.7),
      cv(8.3, 7.2, 2.4), cv(9.5, 1.2, 0.),  cv(1.3, 1.6, 1.8)};

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 3);
}

TEST(TwoDTree, HeightDouble2) {
  CoordinatesValue<double, double> testValues[] = {
      cv(4.95, 0.04, 1.05), cv(5.00, 0.55, 9.68), cv(8.08, 2.43, 3.64),
      cv(6.73, 9.57, 6.48), cv(3.07, 2.18, 5.17), cv(6.43, 4.74, 8.13),
      cv(4.91, 1.74, 0.63), cv(4.31, 3.18, 6.13), cv(0.01, 3.02, 1.26),
      cv(8.13, 2.07, 6.83), cv(7.80, 3.80, 1.01), cv(0.91, 1.76, 9.89),
      cv(6.82, 5.28, 4.96), cv(2.91, 4.91, 5.60), cv(5.82, 8.85, 5.16)};

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 4);
}

TEST(TwoDTree, DotRepresentationInt) {
  CoordinatesValue<int, int> testValues[] = {
      cv(1, 6, 5), cv(2, 5, 6), cv(3, 4, 1),
      cv(4, 3, 2), cv(6, 2, 0), cv(6, 1, 1),
  };

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  std::ofstream out("dottest_int.dot");
  out << tree.toDot();
  out.close();

  SUCCEED();
}

TEST(TwoDTree, DotRepresentationDouble) {
  CoordinatesValue<double, double> testValues[] = {
      cv(8.91, 9.71, 2.41), cv(2.06, 2.38, 0.31), cv(4.78, 0.13, 1.12),
      cv(6.90, 9.48, 7.52), cv(1.45, 3.36, 1.32), cv(6.62, 3.17, 6.28),
      cv(6.70, 1.11, 2.59), cv(5.59, 3.43, 6.52), cv(4.37, 1.89, 0.48),
      cv(6.27, 8.37, 3.87), cv(8.87, 3.25, 9.38), cv(7.28, 0.17, 9.67),
      cv(9.82, 9.92, 1.61), cv(4.92, 5.54, 0.88), cv(1.23, 0.62, 1.97),
      cv(4.09, 4.81, 9.57), cv(3.65, 5.01, 8.09), cv(4.65, 3.48, 9.63),
      cv(7.23, 0.11, 3.13), cv(8.03, 4.38, 0.70), cv(2.70, 4.71, 8.09),
      cv(9.67, 2.63, 6.79), cv(4.90, 8.46, 3.54), cv(2.04, 3.22, 6.19),
      cv(1.49, 6.52, 6.69), cv(4.98, 4.31, 6.33), cv(6.02, 4.05, 5.50),
      cv(8.32, 6.39, 7.08), cv(2.31, 7.93, 8.13), cv(9.88, 3.30, 0.16),
      cv(7.07, 9.53, 4.53), cv(6.91, 8.12, 8.81), cv(5.63, 6.07, 0.69),
      cv(7.08, 2.51, 5.58), cv(1.19, 7.48, 0.75)};

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  std::ofstream out("dottest_double.dot");
  out << tree.toDot();
  out.close();

  SUCCEED();
}

TEST(TwoDTree, NearestInt1) {
  CoordinatesValue<int, int> testValues[] = {cv(0, 0, 0),  cv(1, 1, 10),
                                             cv(2, 2, 20), cv(3, 3, 30),
                                             cv(4, 4, 40), cv(5, 5, 50)};

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  NearestResult<int, int> *result = tree.nearest(Coordinates<int>{5, 6});

  EXPECT_EQ(result->point.x, 5);
  EXPECT_EQ(result->point.y, 5);
  EXPECT_EQ(result->value, 50);
  EXPECT_EQ(result->distance, 1);
  EXPECT_EQ(result->visited, 2);

  SUCCEED();
}

TEST(TwoDTree, NearestDouble1) {
  CoordinatesValue<double, double> testValues[] = {
      cv(0.5, 0.2, 0.),  cv(0.8, 0.8, 10.), cv(2.2, 2.4, 20.),
      cv(4.2, 7.1, 30.), cv(9.2, 4.8, 40.), cv(5.3, 5.2, 50.)};

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  NearestResult<double, double> *result =
      tree.nearest(Coordinates<double>{9.2, 5.8});

  EXPECT_EQ(result->point.x, 9.2);
  EXPECT_EQ(result->point.y, 4.8);
  EXPECT_EQ(result->value, 40);
  EXPECT_EQ(result->distance, 1);
  EXPECT_EQ(result->visited, 3);

  SUCCEED();
}
