#include <array>
#include <cmath>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
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
}

TEST(TwoDTree, AddInt1) {
  CoordinatesValue<int, int> testValues[] = {
      cv(3, 3, 1), cv(2, 3, 7), cv(7, 4, 4), cv(5, 7, 4), cv(5, 5, 3),
      cv(9, 1, 8), cv(4, 5, 8), cv(4, 6, 8), cv(7, 8, 4), cv(7, 6, 6),
      cv(1, 0, 9), cv(7, 9, 6), cv(5, 8, 6), cv(9, 9, 5), cv(8, 0, 2)};

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 4);
  EXPECT_EQ(tree.size(), 15);

  tree.add(cv(4, 4, 2));
  tree.rebalance();

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.size(), 16);
}

TEST(TwoDTree, AddInt2) {
  CoordinatesValue<int, int> testValues[] = {
      cv(6, 1, 2), cv(2, 7, 3), cv(4, 1, 4), cv(7, 8, 6), cv(6, 1, 7),
      cv(2, 2, 0), cv(5, 4, 6), cv(6, 9, 3), cv(7, 2, 6), cv(3, 2, 7),
      cv(2, 4, 2), cv(3, 0, 6), cv(1, 7, 3), cv(3, 2, 3), cv(0, 3, 1),
      cv(3, 2, 9), cv(8, 2, 2), cv(5, 3, 0), cv(2, 4, 2), cv(6, 7, 3),
      cv(3, 1, 3), cv(9, 6, 4), cv(0, 4, 0), cv(6, 6, 3), cv(8, 1, 4)};

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.size(), 25);

  tree.add(cv(5, 2, 4));
  tree.add(cv(8, 0, 5));
  tree.add(cv(7, 6, 9));
  tree.rebalance();

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.size(), 28);
}

TEST(TwoDTree, AddDouble1) {
  CoordinatesValue<double, double> testValues[] = {
      cv(5.69, 6.03, 2.50), cv(6.25, 3.75, 9.64), cv(8.95, 2.50, 9.46),
      cv(6.26, 8.73, 1.94), cv(3.14, 8.07, 8.54), cv(7.06, 9.97, 4.14),
      cv(1.42, 3.78, 4.76), cv(4.42, 0.52, 0.18), cv(3.56, 1.78, 1.77),
      cv(4.20, 7.86, 7.27), cv(1.22, 7.38, 8.26), cv(2.28, 7.71, 4.19),
      cv(7.67, 3.47, 4.20), cv(3.45, 2.83, 1.00), cv(5.84, 8.05, 1.45)};

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 4);
  EXPECT_EQ(tree.size(), 15);

  tree.add(cv(5.60, 2.57, 8.70));
  tree.rebalance();

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.size(), 16);
}

TEST(TwoDTree, AddDouble2) {
  CoordinatesValue<double, double> testValues[] = {
      cv(6.13, 2.89, 3.78), cv(8.49, 4.54, 7.65), cv(1.39, 7.56, 6.75),
      cv(7.29, 6.68, 3.47), cv(4.10, 0.90, 7.07), cv(7.68, 2.27, 3.27),
      cv(2.71, 9.24, 5.85), cv(0.94, 3.44, 8.64), cv(2.19, 5.72, 3.59),
      cv(1.59, 3.18, 3.61), cv(2.87, 1.95, 4.10), cv(4.91, 6.44, 6.17),
      cv(2.04, 4.44, 0.09), cv(7.97, 5.55, 4.26), cv(1.05, 2.27, 4.77),
      cv(3.63, 2.75, 4.12), cv(6.98, 9.22, 4.15), cv(8.16, 5.65, 9.01),
      cv(3.23, 0.47, 6.49), cv(6.91, 8.67, 7.59), cv(0.30, 4.17, 6.10),
      cv(3.09, 7.55, 3.47), cv(2.16, 3.94, 2.37), cv(1.80, 4.56, 4.16),
      cv(5.06, 6.36, 1.86)};

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.size(), 25);

  tree.add(cv(1.64, 8.77, 8.98));
  tree.add(cv(1.54, 0.55, 7.80));
  tree.add(cv(0.31, 7.39, 4.02));
  tree.rebalance();

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.size(), 28);
}

TEST(TwoDTree, AddRangeInt1) {
  CoordinatesValue<int, int> testValues[] = {
      cv(8, 9, 4), cv(9, 3, 5), cv(8, 0, 8), cv(3, 7, 2), cv(7, 7, 6),
      cv(1, 0, 4), cv(0, 8, 6), cv(3, 3, 0), cv(7, 6, 3), cv(3, 4, 5),
      cv(3, 1, 4), cv(2, 9, 7), cv(3, 6, 3), cv(5, 6, 0), cv(4, 7, 5)};

  TwoDTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 4);
  EXPECT_EQ(tree.size(), 15);

  CoordinatesValue<int, int> newValues[] = {
      cv(4, 9, 5), cv(3, 6, 0), cv(9, 9, 1), cv(8, 7, 3), cv(6, 5, 5)};

  tree.addRange(std::begin(newValues), std::end(newValues));
  tree.rebalance();

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.size(), 20);
}

TEST(TwoDTree, AddRangeDouble1) {
  CoordinatesValue<double, double> testValues[] = {
      cv(4.62, 6.65, 9.61), cv(8.74, 9.72, 8.19), cv(5.90, 4.13, 7.78),
      cv(2.11, 1.29, 5.68), cv(4.47, 1.51, 4.45), cv(7.85, 3.34, 5.50),
      cv(7.75, 0.42, 9.08), cv(2.87, 4.49, 9.66), cv(5.45, 8.00, 7.59),
      cv(3.88, 0.62, 3.84), cv(7.47, 4.48, 3.74), cv(4.73, 1.52, 6.73),
      cv(9.45, 4.63, 0.66), cv(1.02, 6.32, 0.28), cv(2.55, 5.17, 3.02),
      cv(0.65, 9.54, 5.82), cv(1.38, 2.82, 0.91), cv(5.91, 8.82, 4.41),
      cv(8.88, 2.25, 8.16), cv(6.50, 1.27, 9.77), cv(8.71, 0.14, 7.01)};

  TwoDTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.size(), 21);

  CoordinatesValue<double, double> newValues[] = {
      cv(5.64, 0.25, 8.54), cv(3.11, 2.58, 9.26), cv(9.97, 0.31, 1.24),
      cv(7.64, 6.85, 4.98), cv(8.01, 3.05, 1.43), cv(1.67, 9.12, 5.13),
      cv(6.10, 5.44, 1.10), cv(0.53, 5.70, 5.11)};

  tree.addRange(std::begin(newValues), std::end(newValues));
  tree.rebalance();

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.size(), 29);
}
