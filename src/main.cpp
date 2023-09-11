#include <algorithm>
#include <cmath>
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <numeric>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include <sstream>

#include "agents/bee.hpp"
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

  options.add_options() ("v,verbose", "Enable debug logging", cxxopts::value<bool>()->default_value("false"))
                        ("e,edge-length", "Edge length of the map",cxxopts::value<unsigned int>()->default_value("1000"))
                        ( "f,flowercount", "flower count of the map", cxxopts::value<unsigned int>()->default_value("10000"))
                        (
      "b,biomes", "Number of biomes to generate",
      cxxopts::value<unsigned int>()->default_value("512"))(
      "r,relaxations", "Number of relaxations to perform",
      cxxopts::value<unsigned int>()->default_value("0"))( "s,seed", "Seed to use for the world generation", cxxopts::value<unsigned int>())
      ("hives", "Number of hives",cxxopts::value<unsigned int>())
      ("t,ticks", "Number of ticks to execute",cxxopts::value<unsigned int>()->default_value("50000"))(
      "json", "Output logs in JSON format",
      cxxopts::value<bool>()->default_value("false"))(
      "benchmark", "Benchmark",
      cxxopts::value<unsigned int>()->default_value("0"));

  cxxopts::ParseResult result = options.parse(argc, argv);

  spdlog::set_level(spdlog::level::info);

  if (result["v"].as<bool>()) {
    spdlog::set_level(spdlog::level::debug);
    spdlog::debug("Debug logging level activated");
  }

  unsigned int benchmarkduration;
  benchmarkduration = result["benchmark"].as<unsigned int>();

  if (result["json"].as<bool>()) {
    // spdlog::set_pattern("{\"message\": \"%v\", \"time\": \"%t\"}");
    spdlog::set_pattern(R"({"message": "%v"})");
  }

  unsigned int num_hives;
  if (result.count("hives")) {
    num_hives = result["hives"].as<unsigned int>();
  } else {
    num_hives = 1;
  }

  unsigned int seed;
  if (result.count("s")) {
    seed = result["s"].as<unsigned int>();
  }
  else{
    seed = 0;
  }

  unsigned int edgeLength = result["e"].as<unsigned int>();
  unsigned int ticks = result["t"].as<unsigned int>();

  ChunkBounds worldBounds{0, edgeLength, 0, edgeLength};

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
  std::unique_ptr<WorldMap> world;

  std::size_t worldCellCount;
  if (rank == 0) {
    spdlog::debug("Starting world generator...");

    GeneratorConfig generatorConfig;
    generatorConfig.biomes = result["b"].as<unsigned int>();
    generatorConfig.seed = seed;
    generatorConfig.size = result["e"].as<unsigned int>();

    WorldGenerator generator(generatorConfig);

    spdlog::debug("World generator started.");

    spdlog::info("Generating world...");
    world = generator.generateWorld();
    spdlog::info("World generated.");

    worldCellCount = world->count();
  }

  if (rank == 0) {
    spdlog::debug("Sending map cell count ({}) to other processes...",
                  worldCellCount);
  }
  MPI_Bcast(&worldCellCount, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

  if (rank != 0) {
    world = std::make_unique<WorldMap>(edgeLength, edgeLength);
    spdlog::debug("Reserved world map memory in {} (size: {} bytes, {} x {})",
                  rank, world->size(), world->dimensions().first,
                  world->dimensions().second);
  }

  if (rank == 0) {
    spdlog::debug("Sending map to other processes...");
  }
  MPI_Bcast(world->get(), static_cast<int>(worldCellCount), MPI_INT, 0,
            MPI_COMM_WORLD);
  if (rank == 0) {
    spdlog::debug("Map sent. Cell count: {}.", worldCellCount);
  } else {
    spdlog::debug("Map received. Size: {}.", worldCellCount);
    world->updateDimensions(edgeLength, edgeLength);
  }

  // Build world state
  spdlog::debug("Initializing chunk {}...", rank);
  ChunkBounds chunkBounds =
      calcualteChunkBounds(edgeLength, edgeLength, processes, rank);
  WorldState state(std::move(world), chunkBounds, worldBounds, processes, rank);

  spdlog::debug("Seeding initial agents...");
  SeedingConfiguration seedingConfig;
  seedingConfig.seed = static_cast<int>(time(nullptr));
  seedingConfig.flowerCount = result["f"].as<unsigned int>();
  seedingConfig.hiveCount = num_hives;
  


  std::vector<AgentTemplate> initialAgents = generateInitialAgents(
      chunkBounds.xMin, chunkBounds.xMax, chunkBounds.yMin, chunkBounds.yMax,
      seedingConfig, edgeLength, seed);

  state.init(initialAgents);

  spdlog::debug("Initial agents seeded.");
  spdlog::debug(
      "Chunk {} with borders [{}, {}) and [{}, {}) and {} agents initialized.",
      rank, chunkBounds.xMin, chunkBounds.xMax, chunkBounds.yMin,
      chunkBounds.yMax, state.agents.count());

  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) {
    spdlog::info("Beginning simulation for {} ticks.", ticks);
  }

  spdlog::info("Agents: {}", state.agents.count());

  spdlog::stopwatch sw;
  for (unsigned int tick = 0; tick <= ticks; ++tick) {

    if (rank == 0 && tick % 100 == 0) {
      spdlog::debug("Current tick: {}", tick);
    }

    if (tick == 3 && rank == 0 && benchmarkduration != 0) {
      sw.reset();
    }

    if (tick == benchmarkduration + 3 && benchmarkduration != 0) {
      if (rank == 0) {
        spdlog::info("Benchmark time: {} ", sw);
      }
      tick = ticks + 1;
    }

    // Basic tick
    std::vector<AgentToTransfer> agentsToTransfer = state.tick();

    // Transfer necessary agents
    for (int receiver = 0; receiver < processes; ++receiver) {
      // Receive bees
      std::vector<Bee> beesToTransfer;
      for (auto it = agentsToTransfer.begin(); it != agentsToTransfer.end();) {
        if (it->targetChunk == receiver &&
            it->agent->gettype() == AgentType::Bee) {
          beesToTransfer.push_back(*std::dynamic_pointer_cast<Bee>(it->agent));
          it = agentsToTransfer.erase(it);
        } else {
          ++it;
        }
      }

      Bee *beesToReceive;

      if (rank == receiver) {
        std::vector<int> sizes(processes);
        std::vector<int> displs(processes);

        for (int sender = 0; sender < processes; ++sender) {
          int count = 0;
          if (sender != receiver) {
            MPI_Recv(&count, 1, MPI_INT, sender, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
          }
          sizes[sender] = sizeof(Bee) * count;
        }

        int currentBytePosition = 0;
        for (int sender = 0; sender < processes; ++sender) {
          displs[sender] = currentBytePosition;
          currentBytePosition += sizes[sender];
        }

        int sum = 0;
        for (auto const &c : sizes) {
          sum += c;
        }

        beesToReceive = new Bee[sum];

        if (sum > 0)
          spdlog::debug("Gathering...");
        MPI_Gatherv(nullptr, 0, MPI_BYTE, beesToReceive, &sizes[0], &displs[0],
                    MPI_BYTE, receiver, MPI_COMM_WORLD);

        if (sum > 0)
          spdlog::debug("Current agent count for chunk {}: {}", rank,
                        state.agents.count());
        for (int i = 0; i < sum / sizeof(Bee); ++i) {
          new (&beesToReceive[i]) Bee;
          auto newBee = std::make_shared<Bee>(beesToReceive[i]);
          newBee->setState(&state);
          PointValue<double, Agent> pvToAdd(newBee->getPosition(), newBee);
          if (i == 0) {
            spdlog::debug("First receiving bee's position: ({}|{})",
                          pvToAdd.value->getPosition().x,
                          pvToAdd.value->getPosition().y);
          }
          state.agents.add(pvToAdd);
        }
        delete[] beesToReceive;

        if (sum > 0)
          spdlog::debug("New agent count for chunk {}: {}", rank,
                        state.agents.count());
      } else {
        auto count = static_cast<int>(beesToTransfer.size());
        MPI_Send(&count, 1, MPI_INT, receiver, 0, MPI_COMM_WORLD);

        if (count > 0) {
          spdlog::debug("First sending bee's position: ({}|{})",
                        beesToTransfer[0].getPosition().x,
                        beesToTransfer[0].getPosition().y);
        }

        int size = count * sizeof(Bee);

        MPI_Gatherv(&beesToTransfer[0], size, MPI_BYTE, nullptr, nullptr,
                    nullptr, MPI_BYTE, receiver, MPI_COMM_WORLD);
      }
    }
    spdlog::debug("Process: {} | Agent count: {}", rank, state.agents.count());
  }

  spdlog::info("Final agent count: {}", state.agents.count());

  MPI_Finalize();

  return EXIT_SUCCESS;
}
