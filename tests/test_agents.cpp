#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <random>

#include "../src/agents/bee.hpp"
#include "../src/agents/hive.hpp"
#include "../src/agents/hive_bee_access.hpp"
#include "../src/utils/point_tree.hpp"
#include "../src/world/chunking.hpp"

using testing::Eq;
using testing::Gt;

TEST(Agents, CreatePointTree) {
  WorldGenerator generator;
  std::unique_ptr<WorldMap> map = generator.generateWorld();
  ChunkBounds worldBounds{0, 1000, 0, 1000};
  WorldState state(std::move(map), worldBounds, worldBounds, 1, 0);

  auto h = std::make_shared<Hive>(state, Coordinates<double>{0, 0});

  h->init(40000, new HiveBeeAccess);

  PointValue<double, Agent> hivePointValue(h->getPosition(), h);
  state.agents.add(hivePointValue);

  EXPECT_THAT(state.agents.count(), Eq(1));

  // h->update();

  // EXPECT_THAT(state.agents.count(), Gt(10));

  std::cout << state.agents.count() << std::endl;
  std::cout << state.agents.height() << std::endl;

  for (int i = 0; i < 1000; ++i) {
    state.tick();
  }

  std::cout << state.agents.count() << std::endl;
  std::cout << state.agents.height() << std::endl;

  std::ofstream out("agent_test.dot");
  out << state.agents.toDot();
  out.close();
}
