#include "worldstate.hpp"

void WorldState::tick() {
  this->agents.traverse(
      [](const PointValue<double, Agent> &pv) { pv.value->update(); });
}

std::pair<int, int> calculateClosestDivisors(int n) {
  int a = std::round(std::sqrt(n));

  while (n % a > 0)
    a -= 1;

  return std::make_pair(a, n / a);
}

ChunkBounds calcualteChunkBounds(int areaMaxX, int areaMaxY, int chunkCount,
                                 int chunkIndex) {
  std::pair<int, int> chunks = calculateClosestDivisors(chunkCount);
  int chunkXCount = chunks.first;
  int chunkYCount = chunks.second;
  int chunkWidth = areaMaxX / chunkXCount;
  int chunkHeight = areaMaxY / chunkYCount;

  int chunkXIndex = chunkIndex % chunkXCount;
  int chunkYIndex = chunkIndex / chunkYCount;

  ChunkBounds result;
  result.xMin = chunkWidth * chunkXIndex;
  result.xMax = std::min(chunkWidth * (chunkXIndex + 1), areaMaxX);
  result.yMin = chunkHeight * chunkYIndex;
  result.yMax = std::min(chunkHeight * (chunkYIndex + 1), areaMaxY);

  return result;
}
