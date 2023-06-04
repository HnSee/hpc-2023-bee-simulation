#ifndef BEESIMULATION_WORLD_GENERATOR_H
#define BEESIMULATION_WORLD_GENERATOR_H

#include "../extern/jc_voronoi.h"
#include <array>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

typedef unsigned char WorldCell;

// Unit is 1x1 Cell is 1m x 1m
typedef std::vector<std::vector<WorldCell>> WorldMap;

enum Biome { Field, Meadow, City, Waters, Forest };

typedef std::array<double, 3> Color;

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
  std::map<Biome, float> biomeProbabilities = {{Biome::Field, 25},
                                               {Biome::Meadow, 25},
                                               {Biome::City, 4},
                                               {Biome::Waters, 1},
                                               {Biome::Forest, 20}};
  std::unordered_map<Biome, Color> biomeColors = {
      {Biome::Field, {0.851, 0.678, 0.067}},
      {Biome::Meadow, {0.118, 0.529, 0.133}},
      {Biome::City, {0.5, 0.5, 0.5}},
      {Biome::Waters, {0.239, 0.314, 0.749}},
      {Biome::Forest, {0, 0.349, 0.047}}};

  unsigned short initialZoom = 32;

  // Per generation variables
  jcv_diagram currentVoronoiRepresentation;
  WorldMap currentWorld;
  unsigned short currentZoom;

  void generateVoronoiRepresentation();
  void generateVoronoiSVG(std::string outputPath);
  void rasterizeVoronoiRepresentation();
  void blurEdges();
  void assignBiomes();
  void generateWorldImage(std::string outputPath);
  void generateWorldImageWithBiomeColor(std::string outputPath);
};

#endif