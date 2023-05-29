#ifndef BEESIMULATION_WORLD_GENERATOR_H
#define BEESIMULATION_WORLD_GENERATOR_H

#include "../extern/jc_voronoi.h"
#include <string>

typedef unsigned char worldCell;

class WorldGenerator {

public:
  WorldGenerator(unsigned int seed, unsigned int edgeLength,
                 unsigned int biomes, unsigned int relaxations);
  ~WorldGenerator();
  worldCell *generateWorld();
  void generateWorldImage(std::string outputPath);

private:
  // Settings
  unsigned int seed;
  unsigned int edgeLength;
  unsigned int biomes;
  unsigned short relaxations;

  // Per generation variables
  jcv_diagram currentVoronoiRepresentation;
  worldCell *currentWorld;
  unsigned short currentZoom;
};

#endif