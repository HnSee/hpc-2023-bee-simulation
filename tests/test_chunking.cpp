#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/world/chunking.hpp"

TEST(Chunking, ChunkPlane1) {
  int chunks = 5;
  int maxX = 10000;
  int maxY = 5000;

  std::vector<ChunkBounds> bounds;

  for (int i = 0; i < chunks; ++i) {
    bounds.push_back(calcualteChunkBounds(maxX, maxY, chunks, i));
  }

  std::ofstream out("chunks.csv");
  out << "id,xmin,xmax,ymin,ymax" << std::endl;

  for (std::size_t i = 0; i < bounds.size(); ++i) {
    out << i << "," << bounds[i].xMin << "," << bounds[i].xMax << ","
        << bounds[i].yMin << "," << bounds[i].yMax << std::endl;
  }

  out.close();

  SUCCEED();
}

TEST(Chunking, GetChunkIdForPoint1) {
  int chunks = 4;
  int maxX = 1000;
  int maxY = 2000;

  double pX = 10;
  double pY = 10;

  int chunkIndex = calcualteChunkIndexOfPoint(maxX, maxY, chunks, pX, pY);

  EXPECT_EQ(chunkIndex, 0);
}

TEST(Chunking, GetChunkIdForPoint2) {
  int chunks = 4;
  int maxX = 1000;
  int maxY = 2000;

  double pX = 600;
  double pY = 600;

  int chunkIndex = calcualteChunkIndexOfPoint(maxX, maxY, chunks, pX, pY);

  EXPECT_EQ(chunkIndex, 1);
}

TEST(Chunking, GetChunkIdForPoint3) {
  int chunks = 4;
  int maxX = 1000;
  int maxY = 2000;

  double pX = 499;
  double pY = 1000;

  int chunkIndex = calcualteChunkIndexOfPoint(maxX, maxY, chunks, pX, pY);

  EXPECT_EQ(chunkIndex, 2);
}

TEST(Chunking, GetChunkIdForPoint4) {
  int chunks = 5;
  int maxX = 10000;
  int maxY = 5000;

  double pX = 7000;
  double pY = 2000;

  int chunkIndex = calcualteChunkIndexOfPoint(maxX, maxY, chunks, pX, pY);

  EXPECT_EQ(chunkIndex, 3);
}
