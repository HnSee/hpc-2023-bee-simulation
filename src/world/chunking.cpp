#include "chunking.hpp"

#include <cmath>

std::pair<int, int> calculateClosestDivisors(int n) {
  int a = std::round(std::sqrt(n));

  while (n % a > 0)
    a -= 1;

  return std::make_pair(n / a, a);
}

ChunkBounds calcualteChunkBounds(int areaMaxX, int areaMaxY, int chunkCount,
                                 int chunkIndex) {
  std::pair<int, int> chunks = calculateClosestDivisors(chunkCount);
  int chunkXCount = chunks.first;
  int chunkYCount = chunks.second;
  int chunkWidth = areaMaxX / chunkXCount;
  int chunkHeight = areaMaxY / chunkYCount;

  int chunkXIndex = chunkIndex % chunkXCount;
  int chunkYIndex = chunkIndex / chunkXCount;

  ChunkBounds result;
  result.xMin = chunkWidth * chunkXIndex;
  result.yMin = chunkHeight * chunkYIndex;

  if (chunkXIndex == chunkXCount - 1) {
    result.xMax = areaMaxX;
  } else {
    result.xMax = chunkWidth * (chunkXIndex + 1);
  }

  if (chunkYIndex == chunkYCount - 1) {
    result.yMax = areaMaxY;
  } else {
    result.yMax = chunkHeight * (chunkYIndex + 1);
  }

  return result;
}
