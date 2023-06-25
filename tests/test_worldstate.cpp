#include <gtest/gtest.h>

#include "../src/world/worldstate.hpp"

TEST(WorldState, CalculateChunkBorders1) {
  int xMax = 1000;
  int yMax = 1000;
  int chunks = 4;

  ChunkBounds c1 = calcualteChunkBounds(xMax, yMax, chunks, 0);
  ChunkBounds c2 = calcualteChunkBounds(xMax, yMax, chunks, 1);
  ChunkBounds c3 = calcualteChunkBounds(xMax, yMax, chunks, 2);
  ChunkBounds c4 = calcualteChunkBounds(xMax, yMax, chunks, 3);

  EXPECT_EQ(c1.xMin, 0);
  EXPECT_EQ(c1.xMax, 500);
  EXPECT_EQ(c1.yMin, 0);
  EXPECT_EQ(c1.yMax, 500);

  EXPECT_EQ(c2.xMin, 500);
  EXPECT_EQ(c2.xMax, 1000);
  EXPECT_EQ(c2.yMin, 0);
  EXPECT_EQ(c2.yMax, 500);

  EXPECT_EQ(c3.xMin, 0);
  EXPECT_EQ(c3.xMax, 500);
  EXPECT_EQ(c3.yMin, 500);
  EXPECT_EQ(c3.yMax, 1000);

  EXPECT_EQ(c4.xMin, 500);
  EXPECT_EQ(c4.xMax, 1000);
  EXPECT_EQ(c4.yMin, 500);
  EXPECT_EQ(c4.yMax, 1000);
}

TEST(WorldState, CalculateChunkBorders2) {
  int xMax = 1000;
  int yMax = 1000;
  int chunks = 5;

  ChunkBounds c1 = calcualteChunkBounds(xMax, yMax, chunks, 0);
  ChunkBounds c2 = calcualteChunkBounds(xMax, yMax, chunks, 1);
  ChunkBounds c3 = calcualteChunkBounds(xMax, yMax, chunks, 2);
  ChunkBounds c4 = calcualteChunkBounds(xMax, yMax, chunks, 3);
  ChunkBounds c5 = calcualteChunkBounds(xMax, yMax, chunks, 4);

  EXPECT_EQ(c1.xMin, 0);
  EXPECT_EQ(c1.xMax, 200);
  EXPECT_EQ(c1.yMin, 0);
  EXPECT_EQ(c1.yMax, 1000);

  EXPECT_EQ(c2.xMin, 200);
  EXPECT_EQ(c2.xMax, 400);
  EXPECT_EQ(c2.yMin, 0);
  EXPECT_EQ(c2.yMax, 1000);

  EXPECT_EQ(c3.xMin, 400);
  EXPECT_EQ(c3.xMax, 600);
  EXPECT_EQ(c3.yMin, 0);
  EXPECT_EQ(c3.yMax, 1000);

  EXPECT_EQ(c4.xMin, 600);
  EXPECT_EQ(c4.xMax, 800);
  EXPECT_EQ(c4.yMin, 0);
  EXPECT_EQ(c4.yMax, 1000);

  EXPECT_EQ(c5.xMin, 800);
  EXPECT_EQ(c5.xMax, 1000);
  EXPECT_EQ(c5.yMin, 0);
  EXPECT_EQ(c5.yMax, 1000);
}

TEST(WorldState, CalculateChunkBorders3) {
  int xMax = 1000;
  int yMax = 1000;
  int chunks = 6;

  ChunkBounds c1 = calcualteChunkBounds(xMax, yMax, chunks, 0);
  ChunkBounds c2 = calcualteChunkBounds(xMax, yMax, chunks, 1);
  ChunkBounds c3 = calcualteChunkBounds(xMax, yMax, chunks, 2);
  ChunkBounds c4 = calcualteChunkBounds(xMax, yMax, chunks, 3);
  ChunkBounds c5 = calcualteChunkBounds(xMax, yMax, chunks, 4);
  ChunkBounds c6 = calcualteChunkBounds(xMax, yMax, chunks, 5);

  EXPECT_EQ(c1.xMin, 0);
  EXPECT_EQ(c1.xMax, 333);
  EXPECT_EQ(c1.yMin, 0);
  EXPECT_EQ(c1.yMax, 500);

  EXPECT_EQ(c2.xMin, 333);
  EXPECT_EQ(c2.xMax, 666);
  EXPECT_EQ(c2.yMin, 0);
  EXPECT_EQ(c2.yMax, 500);

  EXPECT_EQ(c3.xMin, 666);
  EXPECT_EQ(c3.xMax, 1000);
  EXPECT_EQ(c3.yMin, 0);
  EXPECT_EQ(c3.yMax, 500);

  EXPECT_EQ(c4.xMin, 0);
  EXPECT_EQ(c4.xMax, 333);
  EXPECT_EQ(c4.yMin, 500);
  EXPECT_EQ(c4.yMax, 1000);

  EXPECT_EQ(c5.xMin, 333);
  EXPECT_EQ(c5.xMax, 666);
  EXPECT_EQ(c5.yMin, 500);
  EXPECT_EQ(c5.yMax, 1000);

  EXPECT_EQ(c6.xMin, 666);
  EXPECT_EQ(c6.xMax, 1000);
  EXPECT_EQ(c6.yMin, 500);
  EXPECT_EQ(c6.yMax, 1000);
}