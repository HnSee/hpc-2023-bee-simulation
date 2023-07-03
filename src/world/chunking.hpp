#ifndef BEESIMULATION_WORLD_CHUNKING_H
#define BEESIMULATION_WORLD_CHUNKING_H

#include <utility>

/// @brief Struct for representing closed-open chunk borders with [xMin, xMax)
/// and [yMin, yMax)
struct ChunkBounds {
  int xMin;
  int xMax;
  int yMin;
  int yMax;
};

std::pair<int, int> calculateClosestDivisors(int n);

ChunkBounds calcualteChunkBounds(int areaMaxX, int areaMaxY, int chunkCount,
                                 int chunkIndex);

int calcualteChunkIndexOfPoint(int areaMaxX, int areaMaxY,
                                       int chunkCount, double x, double y);

#endif