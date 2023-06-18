#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../src/agents/hive.hpp"
#include "../src/agents/hive_bee_access.hpp"

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