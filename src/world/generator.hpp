#ifndef BEESIMULATION_WORLD_GENERATOR_H
#define BEESIMULATION_WORLD_GENERATOR_H

#include "../extern/jc_voronoi.h"
#include "../utils/ctd_array.hpp"

#include <array>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

enum class Biome : int { Field, Meadow, City, Waters, Forest };
std::ostream &operator<<(std::ostream &os, Biome biome);

struct GeneratorConfig {
  unsigned int seed = 123123123;
  unsigned int size = 1000;
  unsigned int biomes = 512;
};

using WorldCell = Biome;

// Unit is 1x1 Cell is 1m x 1m
using WorldMap = CTDArray<WorldCell>;

using BiomeRegionIdentifier = unsigned char;
using Color = std::array<double, 3>;

class WorldGenerator {

public:
  WorldGenerator(GeneratorConfig &config) : config(config) {}
  std::unique_ptr<WorldMap> generateWorld();

private:
  // Settings
  GeneratorConfig &config;
  unsigned short relaxations = 4;
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
  std::vector<std::vector<unsigned char>> currentWorldBiomeRegions;
  std::unique_ptr<WorldMap> currentWorldMap;
  unsigned short currentZoom;

  void generateVoronoiRepresentation();
  void generateVoronoiSVG(const std::string &outputPath);
  void rasterizeVoronoiRepresentation();
  void blurEdges();
  void freeBiomeRegions();
  void assignBiomes();
  void generateBiomeRegionImage(const std::string &outputPath);
  void generateWorldImageWithBiomeColor(const std::string &outputPath);
};

#endif