#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/utils/ctd_array.hpp"

TEST(CTDArray, Init1) {
  std::size_t rows = 5;
  std::size_t columns = 10;

  CTDArray<int> a(rows, columns);

  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < columns; ++j) {
      a[i][j] = static_cast<int>(i + j);
    }
  }

  EXPECT_EQ(a[1][1], 2);
  EXPECT_EQ(a[4][8], 12);
  EXPECT_EQ(a.size(), sizeof(int) * rows * columns);
}

TEST(CTDArray, Init2) {
  std::size_t rows = 2000;
  std::size_t columns = 2000;

  CTDArray<double> a(rows, columns);

  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < columns; ++j) {
      a[i][j] = static_cast<double>(i * j);
    }
  }

  EXPECT_EQ(a[0][0], 0);
  EXPECT_EQ(a[1999][1999], 3996001);
  EXPECT_EQ(a.size(), sizeof(double) * rows * columns);
}

TEST(CTDArray, ContigousIteration) {
  std::size_t rows = 2000;
  std::size_t columns = 2000;

  CTDArray<int> a(rows, columns);

  for (std::size_t i = 0; i < rows; ++i) {
    for (std::size_t j = 0; j < columns; ++j) {
      a[i][j] = 5;
    }
  }

  for (std::size_t i = 0; i < a.count(); ++i) {
    EXPECT_EQ(a.get()[i], 5);
  }
}
