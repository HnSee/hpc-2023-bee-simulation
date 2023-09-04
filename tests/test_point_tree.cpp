#include <array>
#include <cmath>
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "../src/utils/point_tree.hpp"
#include "matchers.hpp"

using testing::AllOf;
using testing::ByRef;
using testing::Contains;
using testing::Eq;
using testing::Field;
using testing::Matcher;
using testing::Not;

template <typename C, typename V> PointValue<C, V> pv(C x, C y, V v) {
  return PointValue<C, V>(x, y, v);
}

template <typename C, typename V>
PointValue<C, V> pv(C x, C y, std::shared_ptr<V> v) {
  return PointValue<C, V>(Coordinates<C>{x, y}, v);
}

TEST(PointTree, InitInt) {
  PointValue<int, int> testValues[] = {pv(0, 1, 2), pv(5, 3, 6), pv(2, 2, 1),
                                       pv(8, 7, 2), pv(9, 1, 0), pv(1, 1, 1)};

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  SUCCEED();
}

TEST(PointTree, InitDouble) {
  PointValue<double, double> testValues[] = {
      pv(1.5, 2.2, 5.1), pv(5.7, 3.8, 6.2), pv(2.9, 2.8, 1.7),
      pv(8.3, 7.2, 2.4), pv(9.5, 1.2, 0.),  pv(1.3, 1.6, 1.8),
  };

  PointTree<double, double> tree(std::begin(testValues), std::end(testValues));

  SUCCEED();
}

TEST(PointTree, SizeInt1) {
  PointValue<int, int> testValues[] = {
      pv(1, 2, 5), pv(5, 3, 6), pv(2, 2, 1),
      pv(8, 7, 2), pv(9, 1, 0), pv(1, 1, 1),
  };

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.count(), 6);
}

TEST(PointTree, SizeInt2) {
  PointValue<int, int> testValues[] = {
      pv(1, 2, 5), pv(5, 3, 6), pv(2, 2, 1), pv(8, 7, 2), pv(9, 1, 0),
      pv(1, 1, 1), pv(8, 7, 2), pv(9, 1, 0), pv(1, 1, 1), pv(8, 7, 2),
      pv(9, 1, 0), pv(1, 1, 1), pv(8, 7, 2), pv(9, 1, 0), pv(1, 1, 1),
      pv(8, 7, 2), pv(9, 1, 0), pv(1, 1, 1), pv(8, 7, 2), pv(9, 1, 0),
      pv(1, 1, 1), pv(8, 7, 2), pv(9, 1, 0), pv(1, 1, 1), pv(8, 7, 1)};

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.count(), 25);
}

TEST(PointTree, SizeDouble1) {
  PointValue<double, double> testValues[] = {
      pv(1.5, 2.2, 5.1), pv(5.7, 3.8, 6.2), pv(2.9, 2.8, 1.7),
      pv(8.3, 7.2, 2.4), pv(9.5, 1.2, 0.),  pv(1.3, 1.6, 1.8),
  };

  PointTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.count(), 6);
}

TEST(PointTree, SizeDouble2) {
  PointValue<double, double> testValues[] = {
      pv(8.91, 9.71, 2.41), pv(2.06, 2.38, 0.31), pv(4.78, 0.13, 1.12),
      pv(6.90, 9.48, 7.52), pv(1.45, 3.36, 1.32), pv(6.62, 3.17, 6.28),
      pv(6.70, 1.11, 2.59), pv(5.59, 3.43, 6.52), pv(4.37, 1.89, 0.48),
      pv(6.27, 8.37, 3.87), pv(8.87, 3.25, 9.38), pv(7.28, 0.17, 9.67),
      pv(9.82, 9.92, 1.61), pv(4.92, 5.54, 0.88), pv(1.23, 0.62, 1.97),
      pv(4.09, 4.81, 9.57), pv(3.65, 5.01, 8.09), pv(4.65, 3.48, 9.63),
      pv(7.23, 0.11, 3.13), pv(8.03, 4.38, 0.70), pv(2.70, 4.71, 8.09),
      pv(9.67, 2.63, 6.79), pv(4.90, 8.46, 3.54), pv(2.04, 3.22, 6.19),
      pv(1.49, 6.52, 6.69), pv(4.98, 4.31, 6.33), pv(6.02, 4.05, 5.50),
      pv(8.32, 6.39, 7.08), pv(2.31, 7.93, 8.13), pv(9.88, 3.30, 0.16),
      pv(7.07, 9.53, 4.53), pv(6.91, 8.12, 8.81), pv(5.63, 6.07, 0.69),
      pv(7.08, 2.51, 5.58), pv(1.19, 7.48, 0.75)};

  PointTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.count(), 35);
}

TEST(PointTree, HeightInt1) {
  PointValue<int, int> testValues[] = {
      pv(1, 2, 5), pv(5, 3, 6), pv(2, 2, 1), pv(8, 7, 2), pv(9, 1, 0),
      pv(1, 1, 1), pv(8, 7, 2), pv(9, 1, 0), pv(1, 1, 1), pv(8, 7, 2),
      pv(9, 1, 0), pv(1, 1, 1), pv(8, 7, 2), pv(9, 1, 0), pv(1, 1, 1),
      pv(8, 7, 2), pv(9, 1, 0), pv(1, 1, 1), pv(8, 7, 2), pv(9, 1, 0),
      pv(1, 1, 1), pv(8, 7, 2), pv(9, 1, 0), pv(1, 1, 1), pv(8, 7, 1)};

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 5);
}

TEST(PointTree, HeightInt2) {
  PointValue<int, int> testValues[] = {
      pv(1, 2, 5), pv(5, 3, 6), pv(2, 2, 1), pv(8, 7, 2), pv(9, 1, 0),
      pv(1, 1, 1), pv(8, 7, 2), pv(9, 1, 0), pv(1, 1, 1), pv(8, 7, 2),
      pv(1, 1, 1), pv(8, 7, 2), pv(9, 1, 0), pv(1, 1, 1), pv(8, 7, 1)};

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 4);
}

TEST(PointTree, HeightDouble1) {
  PointValue<double, double> testValues[] = {
      pv(1.5, 2.2, 5.1), pv(5.7, 3.8, 6.2), pv(2.9, 2.8, 1.7),
      pv(8.3, 7.2, 2.4), pv(9.5, 1.2, 0.),  pv(1.3, 1.6, 1.8)};

  PointTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 3);
}

TEST(PointTree, HeightDouble2) {
  PointValue<double, double> testValues[] = {
      pv(4.95, 0.04, 1.05), pv(5.00, 0.55, 9.68), pv(8.08, 2.43, 3.64),
      pv(6.73, 9.57, 6.48), pv(3.07, 2.18, 5.17), pv(6.43, 4.74, 8.13),
      pv(4.91, 1.74, 0.63), pv(4.31, 3.18, 6.13), pv(0.01, 3.02, 1.26),
      pv(8.13, 2.07, 6.83), pv(7.80, 3.80, 1.01), pv(0.91, 1.76, 9.89),
      pv(6.82, 5.28, 4.96), pv(2.91, 4.91, 5.60), pv(5.82, 8.85, 5.16)};

  PointTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 4);
}

TEST(PointTree, DotRepresentationInt) {
  PointValue<int, int> testValues[] = {
      pv(1, 6, 5), pv(2, 5, 6), pv(3, 4, 1),
      pv(4, 3, 2), pv(6, 2, 0), pv(6, 1, 1),
  };

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  std::ofstream out("dottest_int.dot");
  out << tree.toDot();
  out.close();

  SUCCEED();
}

TEST(PointTree, DotRepresentationDouble) {
  PointValue<double, double> testValues[] = {
      pv(8.91, 9.71, 2.41), pv(2.06, 2.38, 0.31), pv(4.78, 0.13, 1.12),
      pv(6.90, 9.48, 7.52), pv(1.45, 3.36, 1.32), pv(6.62, 3.17, 6.28),
      pv(6.70, 1.11, 2.59), pv(5.59, 3.43, 6.52), pv(4.37, 1.89, 0.48),
      pv(6.27, 8.37, 3.87), pv(8.87, 3.25, 9.38), pv(7.28, 0.17, 9.67),
      pv(9.82, 9.92, 1.61), pv(4.92, 5.54, 0.88), pv(1.23, 0.62, 1.97),
      pv(4.09, 4.81, 9.57), pv(3.65, 5.01, 8.09), pv(4.65, 3.48, 9.63),
      pv(7.23, 0.11, 3.13), pv(8.03, 4.38, 0.70), pv(2.70, 4.71, 8.09),
      pv(9.67, 2.63, 6.79), pv(4.90, 8.46, 3.54), pv(2.04, 3.22, 6.19),
      pv(1.49, 6.52, 6.69), pv(4.98, 4.31, 6.33), pv(6.02, 4.05, 5.50),
      pv(8.32, 6.39, 7.08), pv(2.31, 7.93, 8.13), pv(9.88, 3.30, 0.16),
      pv(7.07, 9.53, 4.53), pv(6.91, 8.12, 8.81), pv(5.63, 6.07, 0.69),
      pv(7.08, 2.51, 5.58), pv(1.19, 7.48, 0.75)};

  PointTree<double, double> tree(std::begin(testValues), std::end(testValues));

  std::ofstream out("dottest_double.dot");
  out << tree.toDot();
  out.close();

  SUCCEED();
}

TEST(PointTree, NearestInt1) {
  PointValue<int, int> testValues[] = {pv(0, 0, 0),  pv(1, 1, 10),
                                       pv(2, 2, 20), pv(3, 3, 30),
                                       pv(4, 4, 40), pv(5, 5, 50)};

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  NearestResult<int, int> *result = tree.nearest(Coordinates<int>{5, 6});

  EXPECT_EQ(result->point.x, 5);
  EXPECT_EQ(result->point.y, 5);
  EXPECT_EQ(result->value, 50);
  EXPECT_EQ(result->distance, 1);
  EXPECT_EQ(result->visited, 2);

  delete result;
}

TEST(PointTree, NearestDouble1) {
  PointValue<double, double> testValues[] = {
      pv(0.5, 0.2, 0.),  pv(0.8, 0.8, 10.), pv(2.2, 2.4, 20.),
      pv(4.2, 7.1, 30.), pv(9.2, 4.8, 40.), pv(5.3, 5.2, 50.)};

  PointTree<double, double> tree(std::begin(testValues), std::end(testValues));

  NearestResult<double, double> *result =
      tree.nearest(Coordinates<double>{9.2, 5.8});

  EXPECT_EQ(result->point.x, 9.2);
  EXPECT_EQ(result->point.y, 4.8);
  EXPECT_EQ(result->value, 40);
  EXPECT_EQ(result->distance, 1);
  EXPECT_EQ(result->visited, 3);

  delete result;
}

TEST(PointTree, AddInt1) {
  PointValue<int, int> testValues[] = {
      pv(3, 3, 1), pv(2, 3, 7), pv(7, 4, 4), pv(5, 7, 4), pv(5, 5, 3),
      pv(9, 1, 8), pv(4, 5, 8), pv(4, 6, 8), pv(7, 8, 4), pv(7, 6, 6),
      pv(1, 0, 9), pv(7, 9, 6), pv(5, 8, 6), pv(9, 9, 5), pv(8, 0, 2)};

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 4);
  EXPECT_EQ(tree.count(), 15);

  tree.add(pv(4, 4, 2));
  tree.rebalance();

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.count(), 16);
}

TEST(PointTree, AddInt2) {
  PointValue<int, int> testValues[] = {
      pv(6, 1, 2), pv(2, 7, 3), pv(4, 1, 4), pv(7, 8, 6), pv(6, 1, 7),
      pv(2, 2, 0), pv(5, 4, 6), pv(6, 9, 3), pv(7, 2, 6), pv(3, 2, 7),
      pv(2, 4, 2), pv(3, 0, 6), pv(1, 7, 3), pv(3, 2, 3), pv(0, 3, 1),
      pv(3, 2, 9), pv(8, 2, 2), pv(5, 3, 0), pv(2, 4, 2), pv(6, 7, 3),
      pv(3, 1, 3), pv(9, 6, 4), pv(0, 4, 0), pv(6, 6, 3), pv(8, 1, 4)};

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.count(), 25);

  tree.add(pv(5, 2, 4));
  tree.add(pv(8, 0, 5));
  tree.add(pv(7, 6, 9));
  tree.rebalance();

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.count(), 28);
}

TEST(PointTree, AddDouble1) {
  PointValue<double, double> testValues[] = {
      pv(5.69, 6.03, 2.50), pv(6.25, 3.75, 9.64), pv(8.95, 2.50, 9.46),
      pv(6.26, 8.73, 1.94), pv(3.14, 8.07, 8.54), pv(7.06, 9.97, 4.14),
      pv(1.42, 3.78, 4.76), pv(4.42, 0.52, 0.18), pv(3.56, 1.78, 1.77),
      pv(4.20, 7.86, 7.27), pv(1.22, 7.38, 8.26), pv(2.28, 7.71, 4.19),
      pv(7.67, 3.47, 4.20), pv(3.45, 2.83, 1.00), pv(5.84, 8.05, 1.45)};

  PointTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 4);
  EXPECT_EQ(tree.count(), 15);

  tree.add(pv(5.60, 2.57, 8.70));
  tree.rebalance();

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.count(), 16);
}

TEST(PointTree, AddDouble2) {
  PointValue<double, double> testValues[] = {
      pv(6.13, 2.89, 3.78), pv(8.49, 4.54, 7.65), pv(1.39, 7.56, 6.75),
      pv(7.29, 6.68, 3.47), pv(4.10, 0.90, 7.07), pv(7.68, 2.27, 3.27),
      pv(2.71, 9.24, 5.85), pv(0.94, 3.44, 8.64), pv(2.19, 5.72, 3.59),
      pv(1.59, 3.18, 3.61), pv(2.87, 1.95, 4.10), pv(4.91, 6.44, 6.17),
      pv(2.04, 4.44, 0.09), pv(7.97, 5.55, 4.26), pv(1.05, 2.27, 4.77),
      pv(3.63, 2.75, 4.12), pv(6.98, 9.22, 4.15), pv(8.16, 5.65, 9.01),
      pv(3.23, 0.47, 6.49), pv(6.91, 8.67, 7.59), pv(0.30, 4.17, 6.10),
      pv(3.09, 7.55, 3.47), pv(2.16, 3.94, 2.37), pv(1.80, 4.56, 4.16),
      pv(5.06, 6.36, 1.86)};

  PointTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.count(), 25);

  tree.add(pv(1.64, 8.77, 8.98));
  tree.add(pv(1.54, 0.55, 7.80));
  tree.add(pv(0.31, 7.39, 4.02));
  tree.rebalance();

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.count(), 28);
}

TEST(PointTree, AddRangeInt1) {
  PointValue<int, int> testValues[] = {
      pv(8, 9, 4), pv(9, 3, 5), pv(8, 0, 8), pv(3, 7, 2), pv(7, 7, 6),
      pv(1, 0, 4), pv(0, 8, 6), pv(3, 3, 0), pv(7, 6, 3), pv(3, 4, 5),
      pv(3, 1, 4), pv(2, 9, 7), pv(3, 6, 3), pv(5, 6, 0), pv(4, 7, 5)};

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 4);
  EXPECT_EQ(tree.count(), 15);

  PointValue<int, int> newValues[] = {pv(4, 9, 5), pv(3, 6, 0), pv(9, 9, 1),
                                      pv(8, 7, 3), pv(6, 5, 5)};

  tree.addRange(std::begin(newValues), std::end(newValues));
  tree.rebalance();

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.count(), 20);
}

TEST(PointTree, AddRangeDouble1) {
  PointValue<double, double> testValues[] = {
      pv(4.62, 6.65, 9.61), pv(8.74, 9.72, 8.19), pv(5.90, 4.13, 7.78),
      pv(2.11, 1.29, 5.68), pv(4.47, 1.51, 4.45), pv(7.85, 3.34, 5.50),
      pv(7.75, 0.42, 9.08), pv(2.87, 4.49, 9.66), pv(5.45, 8.00, 7.59),
      pv(3.88, 0.62, 3.84), pv(7.47, 4.48, 3.74), pv(4.73, 1.52, 6.73),
      pv(9.45, 4.63, 0.66), pv(1.02, 6.32, 0.28), pv(2.55, 5.17, 3.02),
      pv(0.65, 9.54, 5.82), pv(1.38, 2.82, 0.91), pv(5.91, 8.82, 4.41),
      pv(8.88, 2.25, 8.16), pv(6.50, 1.27, 9.77), pv(8.71, 0.14, 7.01)};

  PointTree<double, double> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.count(), 21);

  PointValue<double, double> newValues[] = {
      pv(5.64, 0.25, 8.54), pv(3.11, 2.58, 9.26), pv(9.97, 0.31, 1.24),
      pv(7.64, 6.85, 4.98), pv(8.01, 3.05, 1.43), pv(1.67, 9.12, 5.13),
      pv(6.10, 5.44, 1.10), pv(0.53, 5.70, 5.11)};

  tree.addRange(std::begin(newValues), std::end(newValues));
  tree.rebalance();

  EXPECT_EQ(tree.height(), 5);
  EXPECT_EQ(tree.count(), 29);
}

TEST(PointTree, RemoveLeaf) {
  PointValue<int, int> testValues[] = {
      pv(6, 42, 6),   pv(87, 39, 56), pv(54, 22, 62), pv(9, 14, 81),
      pv(40, 13, 20), pv(13, 97, 87), pv(96, 22, 89), pv(21, 78, 96),
      pv(54, 39, 89), pv(2, 45, 2),   pv(51, 21, 43), pv(72, 90, 1),
      pv(99, 80, 96), pv(80, 18, 2),  pv(58, 24, 6)};

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.count(), 15);

  PointValue<int, int> valueToRemove = pv(9, 14, testValues[3].value);

  std::ofstream out("remove_test.dot");
  out << tree.toDot();
  out.close();

  tree.removeByPointValue(valueToRemove);

  EXPECT_EQ(tree.count(), 14);
}

TEST(PointTree, RemoveRoot) {
  PointValue<int, int> testValues[] = {
      pv(6, 42, 6),   pv(87, 39, 56), pv(54, 22, 62), pv(9, 14, 81),
      pv(40, 13, 20), pv(13, 97, 87), pv(96, 22, 89), pv(21, 78, 96),
      pv(54, 39, 89), pv(2, 45, 2),   pv(51, 21, 43), pv(72, 90, 1),
      pv(99, 80, 96), pv(80, 18, 2),  pv(58, 24, 6)};

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.count(), 15);

  PointValue<int, int> valueToRemove = pv(54, 22, testValues[2].value);

  std::ofstream out("remove_test_before.dot");
  out << tree.toDot();
  out.close();

  tree.removeByPointValue(valueToRemove);

  EXPECT_EQ(tree.count(), 14);

  std::ofstream out2("remove_test_after.dot");
  out2 << tree.toDot();
  out2.close();
}

TEST(PointTree, RemoveNodeWithoutRightChild) {
  PointValue<int, int> testValues[] = {
      pv(6, 42, 6),   pv(87, 39, 56), pv(54, 22, 62), pv(9, 14, 81),
      pv(40, 15, 20), pv(13, 97, 87), pv(96, 22, 89), pv(21, 78, 96),
      pv(54, 39, 89), pv(2, 45, 2),   pv(51, 21, 43), pv(72, 90, 1),
      pv(99, 80, 96), pv(80, 18, 2),  pv(58, 24, 6),  pv(58, 22, 69)};

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.count(), 16);

  PointValue<int, int> valueToAdd = pv(31, 60, 1010);
  PointValue<int, int> valueToRemove = pv(21, 78, testValues[7].value);

  tree.add(valueToAdd);

  std::ofstream out("remove_test_without_right_child_before.dot");
  out << tree.toDot();
  out.close();

  tree.removeByPointValue(valueToRemove);

  std::ofstream out2("remove_test_without_right_child_after.dot");
  out2 << tree.toDot();
  out2.close();

  EXPECT_EQ(tree.count(), 16);
}

TEST(PointTree, RangeQuery1) {
  PointValue<int, int> testValues[] = {
      pv(72, 19, 27), pv(79, 51, 73), pv(4, 68, 6),   pv(91, 53, 65),
      pv(77, 67, 30), pv(31, 67, 25), pv(29, 4, 86),  pv(36, 85, 56),
      pv(69, 87, 83), pv(24, 43, 24), pv(81, 69, 94), pv(33, 99, 85),
      pv(63, 75, 60), pv(86, 13, 39), pv(17, 78, 2),  pv(68, 70, 7)};

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.count(), 16);

  Coordinates<int> center{20, 40};
  double range = 10;

  std::unique_ptr<RangeResult<int, int>> result = tree.range(center, range);

  std::ofstream out("rangeSearchTest1.csv");
  out << tree.toCsvWithParents();
  out.close();

  EXPECT_EQ(result->size(), 1);

  EXPECT_EQ(result->at(0).point.x, 24);
  EXPECT_EQ(result->at(0).point.y, 43);
  EXPECT_EQ(*result->at(0).value, 24);
  EXPECT_EQ(result->at(0).distance, 5);
}

TEST(PointTree, RangeQuery2) {
  PointValue<int, int> testValues[] = {
      pv(90, 39, 34), pv(24, 53, 79), pv(13, 18, 90), pv(51, 69, 52),
      pv(8, 54, 62),  pv(12, 9, 25),  pv(76, 59, 9),  pv(80, 85, 45),
      pv(3, 52, 83),  pv(13, 70, 23), pv(6, 69, 0),   pv(95, 28, 52),
      pv(78, 96, 35), pv(82, 31, 35), pv(45, 89, 42), pv(72, 87, 69)};

  PointTree<int, int> tree(std::begin(testValues), std::end(testValues));

  EXPECT_EQ(tree.count(), 16);

  Coordinates<int> center{15, 70};
  double range = 40;

  std::unique_ptr<RangeResult<int, int>> result = tree.range(center, range);

  std::ofstream out("rangeSearchTest2.csv");
  out << tree.toCsvWithParents();
  out.close();

  EXPECT_EQ(result->size(), 7);

  EXPECT_THAT(*result, Contains(EqRangeResult(3, 52, 83)));
  EXPECT_THAT(*result, Contains(EqRangeResult(6, 69, 0)));
  EXPECT_THAT(*result, Contains(EqRangeResult(8, 54, 62)));
  EXPECT_THAT(*result, Contains(EqRangeResult(13, 70, 23)));
  EXPECT_THAT(*result, Contains(EqRangeResult(24, 53, 79)));
  EXPECT_THAT(*result, Contains(EqRangeResult(45, 89, 42)));
  EXPECT_THAT(*result, Contains(EqRangeResult(51, 69, 52)));

  EXPECT_THAT(*result, Not(Contains(EqRangeResult(13, 18, 90))));
}
