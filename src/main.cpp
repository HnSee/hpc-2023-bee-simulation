#include <algorithm>
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <numeric>
#include <spdlog/spdlog.h>
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

  options.add_options()("v,verbose", "Enable debug logging",
                        cxxopts::value<bool>()->default_value("false"))(
      "e,edge-length", "Edge length of the map",
      cxxopts::value<unsigned int>()->default_value("1000"))(
      "b,biomes", "Number of biomes to generate",
      cxxopts::value<unsigned int>()->default_value("100"))(
      "r,relaxations", "Number of relaxations to perform",
      cxxopts::value<unsigned int>()->default_value("1"))(
      "s,seed", "Seed to use for the world generation",
      cxxopts::value<unsigned int>());

  cxxopts::ParseResult result = options.parse(argc, argv);

  spdlog::set_level(spdlog::level::debug);spdlog::set_level(spdlog::level::debug);
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
  unsigned int ticks = 50000;

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
    WorldGenerator generator;
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

  // TODO: Seed once and distribute to processes
  spdlog::debug("Seeding initial agents...");
  SeedingConfiguration seedingConfig;
  seedingConfig.seed = static_cast<int>(time(nullptr));
  seedingConfig.flowerCount = 5;
  seedingConfig.hiveCount = 0;

  if (rank == 0) {
    seedingConfig.hiveCount = 1;
  }

  std::srand(50);
  std::vector<AgentTemplate> initialAgents =
      generateInitialAgents(chunkBounds.xMin, chunkBounds.xMax,
                            chunkBounds.yMin, chunkBounds.yMax, seedingConfig);
  state.init(initialAgents);
  spdlog::debug("Initial agents seeded.");
  spdlog::debug(
      "Chunk {} with borders [{}, {}) and [{}, {}) and {} agents initialized.",
      rank, chunkBounds.xMin, chunkBounds.xMax, chunkBounds.yMin,
      chunkBounds.yMax, state.agents.count());

  MPI_Barrier(MPI_COMM_WORLD);

  for (unsigned int tick = 0; tick <= ticks; ++tick) {
    if (rank == 0 && tick % 100 == 0) {
      spdlog::debug("Current tick: {}", tick);
    }

    // Basic tick
    std::vector<AgentToTransfer> agentsToTransfer = state.tick();

    std::string a = state.agents.toCsv();
    
    std::ofstream myfile;
    long filename = 1000000+tick;
    spdlog::debug("src/visualization/csv/" + std::to_string(filename));
    myfile.open ( "src/visualization/csv/" + std::to_string(filename));
    myfile << a;
    myfile.close();

    spdlog::debug("Vector len: {}", agentsToTransfer.size());

    // Transfer necessary agents
    for (int receiver = 0; receiver < processes; ++receiver) {
      // Receive bees
      std::vector<std::shared_ptr<Bee>> beesToTransfer;
      for (auto it = agentsToTransfer.begin(); it != agentsToTransfer.end();) {
        if (it->first == receiver && it->second->gettype() == AgentType::Bee) {
          beesToTransfer.emplace_back(
              std::dynamic_pointer_cast<Bee>(it->second));
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
          displs[sender] = sizes[sender] * sender;
        }

        int sum = 0;
        for (auto &c : sizes) {
          sum += c;
        }

        if (sum > 0) {
          spdlog::info("Rank {} is about to receive {} bees:", rank, sum);
          for (int i = 0; i < sizes.size(); ++i) {
            spdlog::info("  {} from rank {}, inserted at {}", sizes[i], i,
                         displs[i]);
          }
        }
  
        beesToReceive = new Bee[sum];

        if (sum > 0)
          spdlog::debug("Gathering...");
        MPI_Gatherv(nullptr, 0, MPI_BYTE, beesToReceive, &sizes[0], &displs[0],
                    MPI_BYTE, receiver, MPI_COMM_WORLD);

        if (sum > 0)
          spdlog::debug("Current agent count for chunk {}: {}", rank,
                        state.agents.count());
        for (int i = 0; i < sum; ++i) {
          // spdlog::debug("Creating bee: ({}|{}), {}",
          // beesToReceive[i].hivepos.x,
          //               beesToReceive[i].hivepos.y,
          //               beesToReceive[i].searching);
          new (&beesToReceive[i]) Bee;
          std::shared_ptr<Bee> newBee = std::make_shared<Bee>(beesToReceive[i]);
          // spdlog::debug("Bee created");
          newBee->setState(&state);
          PointValue<double, Agent> pvToAdd(newBee->getPosition(), newBee);
          if (i == 0) {
            spdlog::info("First receiving bee's position: ({}|{})",
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
        int count = static_cast<int>(beesToTransfer.size());
        MPI_Send(&count, 1, MPI_INT, receiver, 0, MPI_COMM_WORLD);

        if (count > 0) {
          spdlog::info("First sending bee's position: ({}|{})",
                       beesToTransfer[0]->getPosition().x,
                       beesToTransfer[0]->getPosition().y);
        }

        std::vector<Bee_struct> transferdata;
        for(int k = 0; k < beesToTransfer.size(); k++){
          transferdata.push_back( beesToTransfer[k]->get_struct() );
        }

        MPI_Gatherv(&beesToTransfer[0], count, MPI_BYTE, nullptr, nullptr,
                    nullptr, MPI_BYTE, receiver, MPI_COMM_WORLD);
      }

      // MPI_Gatherv(&agentsToSend[0], sizeof(agentsToSend), MPI_BYTE, );
    }
  }

  spdlog::info("Final agent count: {}", state.agents.count());

  MPI_Finalize();

  return EXIT_SUCCESS;
}
