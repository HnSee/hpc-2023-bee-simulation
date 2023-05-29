#include <cxxopts.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

#include "world/generator.hpp"

int main(int argc, char **argv) {
  cxxopts::Options options("BeeSimulation", "Agent-based simulation of bees");

  options.add_options()("v,verbose", "Enable debug logging",
                        cxxopts::value<bool>()->default_value("false"))(
      "e,edge-length", "Edge length of the map",
      cxxopts::value<unsigned int>()->default_value("200000"))(
      "b,biomes", "Number of biomes to generate",
      cxxopts::value<unsigned int>()->default_value("100"))(
      "r,relaxations", "Number of relaxations to perform",
      cxxopts::value<unsigned int>()->default_value("1"))(
      "s,seed", "Seed to use for the world generation",
      cxxopts::value<unsigned int>());

  cxxopts::ParseResult result = options.parse(argc, argv);

  if (result["v"].as<bool>()) {
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("Debug logging level activated");
  }

  unsigned int seed;
  if (result.count("s")) {
    seed = result["s"].as<unsigned int>();
  } else {
    seed = (unsigned)time(NULL);
  }

  unsigned int edgeLength = result["e"].as<unsigned int>();
  unsigned int biomes = result["b"].as<unsigned int>();
  unsigned int relaxations = result["r"].as<unsigned int>();

  spdlog::debug("Starting world generator...");
  WorldGenerator generator;
  spdlog::debug("World generator started.");

  spdlog::info("Generating world...");
  WorldMap *world = generator.generateWorld();
  spdlog::info("World generated.");

  // spdlog::info("Generating world image...");
  // generator.generateWorldImage("output.svg");
  // spdlog::info("World image generated.");

  return EXIT_SUCCESS;
}
