#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <random>

#include "../src/agents/bee.hpp"
#include "../src/agents/hive.hpp"
#include "../src/agents/hive_bee_access.hpp"
#include "../src/utils/point_tree.hpp"

// remove selfpointer
TEST(Agents, Init) {
  std::srand(time(NULL));

  Hive *h = new Hive;

  h->init(40000, &agents, new HiveBeeAccess);

  agents.push_back(h);

  for (int x = 0; x < 5; x++) {
    for (int k = 0; k < daylength; k++) {
      auto t = agents.size();
      h->tickoftheday = 0;
      for (int j = 0; j < t; j++) {
        tick(j);
        t = agents.size();
      }
      // baum aufgebaut werden
    }

    std::cout << "Scout: " << scout << "\n";
    std::cout << "Worker: " << worker << "\n";
  }

  SUCCEED();
}

TEST(Agents, CreatePointTree) {
  Hive *h = new Hive;

  std::vector<Agent *> temporaryAgents;
  h->init(40000, &temporaryAgents, new HiveBeeAccess);

  temporaryAgents.push_back(h);

  std::uniform_real_distribution<double> distr(0, 100);
  std::default_random_engine re;

  for (int i = 0; i < 40000; ++i) {
    Bee *b = new Bee;
    Coordinates<double> pos{distr(re), distr(re)};
    Coordinates<double> dest{distr(re), distr(re)};
    b->init(pos, h->pos, dest, false, true, h->hstore);
    temporaryAgents.push_back(b);
  }

  std::vector<PointValue<double, Agent>> values;

  for (auto agent : temporaryAgents) {
    values.emplace_back(agent->pos, std::shared_ptr<Agent>(agent));
  }

  PointTree<double, Agent> agentsAsTree(values.begin(), values.end());

  EXPECT_EQ(agentsAsTree.count(), 40001);
  EXPECT_EQ(agentsAsTree.height(), 16);

  std::ofstream out("agent_test.dot");
  out << agentsAsTree.toDot();
  out.close();
}
