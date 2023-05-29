#ifndef BEESIMULATION_WORLD_GENERATOR_H
#define BEESIMULATION_WORLD_GENERATOR_H

#include "../extern/jc_voronoi.h"
#include <string>
#include <vector>

typedef unsigned char WorldCell;

// Unit is 1x1 Cell is 1m x 1m
typedef std::vector<std::vector<WorldCell>> WorldMap;

class WorldGenerator {

public:
  // TODO: move settings in this constructor instead of hardcoded values below
  // WorldGenerator();
  WorldMap *generateWorld();

private:
  // Settings
  unsigned int seed = 123123123;
  unsigned int size = 1024;
  unsigned int biomes = 512;
  unsigned short relaxations = 4;
  // double perlinPersistence = 0.5;
  int perlinOctaves = 8;
  unsigned short edgeDisplacement = 12;

  unsigned short initialZoom = 32;

  // Per generation variables
  jcv_diagram currentVoronoiRepresentation;
  WorldMap currentWorld;
  unsigned short currentZoom;

  void generateVoronoiRepresentation();
  void generateVoronoiSVG(std::string outputPath);
  void rasterizeVoronoiRepresentation();
  void blurEdges();
  void generateWorldImage(std::string outputPath);
};

#endif