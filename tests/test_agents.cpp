#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <random>

#include "../src/agents/bee.hpp"
#include "../src/agents/hive.hpp"
#include "../src/agents/hive_bee_access.hpp"
#include "../src/utils/point_tree.hpp"

using testing::Eq;
using testing::Gt;

TEST(Agents, CreatePointTree) {
  WorldGenerator generator;
  WorldMap *map = generator.generateWorld();
  WorldState state(map);

  std::shared_ptr<Hive> h = std::make_shared<Hive>(state);

  h->init(40000, new HiveBeeAccess);

  PointValue<double, Agent> hivePointValue(h->pos, h);
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
