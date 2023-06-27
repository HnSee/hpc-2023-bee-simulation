#include <algorithm>
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <numeric>
#include <spdlog/spdlog.h>
#include <sstream>

#include "world/chunking.hpp"
#include "world/generator.hpp"
#include "world/seeding.hpp"
#include "world/worldstate.hpp"

void checkForMPIError(int err, std::string action) {
  if (err != 0) {
    spdlog::error("MPI error ({}, Code: {})", action, err);
    exit(1);
  }
}

int main(int argc, char **argv) {
  cxxopts::Options options("BeeSimulation", "Agent-based simulation of bees");

  options.add_options()("v,verbose", "Enable debug logging",
                        cxxopts::value<bool>()->default_value("false"))(
      "e,edge-length", "Edge length of the map",
      cxxopts::value<unsigned int>()->default_value("10000"))(
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

  // TODO: EXTERNALISE CONFIGURATION
  unsigned int edgeLength = result["e"].as<unsigned int>();
  unsigned int biomes = result["b"].as<unsigned int>();
  unsigned int relaxations = result["r"].as<unsigned int>();

  // Initialize MPI
  int mpiErr = MPI_Init(&argc, &argv);
  checkForMPIError(mpiErr, "Initialization");

  int processes;
  mpiErr = MPI_Comm_size(MPI_COMM_WORLD, &processes);
  checkForMPIError(mpiErr, "Receiving number of processes");

  int rank;
  mpiErr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  checkForMPIError(mpiErr, "Receiving rank");

  if (rank == 0) {
    spdlog::debug("Starting with {} MPI process(es)", processes);
  }

  spdlog::debug("Initialized MPI process {}", rank);

  // Build world map
  WorldMap world;
  WorldCell *worldSerialized;
  std::size_t worldSerializedSize;
  if (rank == 0) {
    spdlog::debug("Starting world generator...");
    WorldGenerator generator;
    spdlog::debug("World generator started.");

    spdlog::info("Generating world...");
    world = generator.generateWorld();
    spdlog::info("World generated.");

    std::pair<WorldCell *, std::size_t> worldSerializedResult =
        serializeWorldMap(world);
    worldSerialized = worldSerializedResult.first;
    worldSerializedSize = worldSerializedResult.second;
  }

  if (rank == 0) {
    spdlog::debug("Sending map size ({}) to other processes...",
                  worldSerializedSize);
  }
  MPI_Bcast(&worldSerializedSize, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

  if (rank != 0) {
    worldSerialized = new WorldCell[worldSerializedSize];
  }

  if (rank == 0) {
    spdlog::debug("Sending map to other processes...",
                  worldSerializedSize);
  }
  MPI_Bcast(worldSerialized, worldSerializedSize, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank == 0) {
    spdlog::debug("Map sent. Size: {}.", worldSerializedSize);
  } else {
    spdlog::debug("Map received. Size: {}.", worldSerializedSize);

    spdlog::debug("Deserializing map...");
    std::pair<WorldCell *, std::size_t> serializedMapData =
        std::make_pair(worldSerialized, worldSerializedSize);
    world = deserializeWorldMap(serializedMapData, edgeLength);
    spdlog::debug("Map deserialized.");
  }

  std::stringstream fileName;
  fileName << "map_from_rank_" << rank << ".txt";
  std::ofstream f(fileName.str());
  for (auto &r : world) {
    for (auto &c : r) {
      f << c << " ";
    }
    f << std::endl;
  }
  f.close();

  // ChunkBounds chunkBounds =
  //     calcualteChunkBounds(edgeLength, edgeLength, processes, rank);
  // WorldState *state;
  // if (rank == 0) {
  //   spdlog::debug("Seeding initial agents...");

  //   SeedingConfiguration seedingConfig;
  //   seedingConfig.seed = time(NULL);
  //   seedingConfig.flowerCount = 500;
  //   seedingConfig.hiveCount = 4;

  //   std::vector<AgentTemplate> initialAgents =
  //       generateInitialAgents(1000, 1000, seedingConfig);
  //   std::vector<std::vector<AgentTemplate>> initialAgentsPerChunk =
  //       partitionInitialAgentsIntoChunks(initialAgents, edgeLength,
  //       edgeLength,
  //                                        processes);

  //   spdlog::debug("Initial agents seeded.");

  //   spdlog::debug("Initializing state for chunk {}", rank);
  //   state = new WorldState(world, chunkBounds);
  // }

  // spdlog::info("Generating world image...");
  // generator.generateWorldImage("output.svg");
  // spdlog::info("World image generated.");

  // ChunkBounds chunkBoundsForThisProcess =
  //     calcualteChunkBounds(world->size(), world[0].size(), processes, rank);
  // WorldState state(
  //     world, chunkBoundsForThisProcess.xMin, chunkBoundsForThisProcess.xMax,
  //     chunkBoundsForThisProcess.yMin, chunkBoundsForThisProcess.yMax);

  MPI_Finalize();

  return EXIT_SUCCESS;
}
