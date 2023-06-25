#include "../src/utils/point_tree.hpp"
#include <algorithm>
#include <benchmark/benchmark.h>
#include <cstddef>
#include <random>
#include <vector>

typedef double CoordinateType;
typedef double ValueType;

static Coordinates<CoordinateType> constructRandomCoordinates(std::size_t min,
                                                              std::size_t max) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<CoordinateType> uniformCoordinates(min, max);

  Coordinates<CoordinateType> randomCoordinate{uniformCoordinates(rng),
                                               uniformCoordinates(rng)};

  return randomCoordinate;
}

static PointValue<CoordinateType, ValueType>
constructRandomPointValue(std::size_t minCoordinate, std::size_t maxCoordinate,
                          std::size_t minValue, std::size_t maxValue) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<CoordinateType> uniformCoordinates(
      minCoordinate, maxCoordinate);
  std::uniform_real_distribution<ValueType> uniformValues(minValue, maxValue);

  CoordinateType randomX = uniformCoordinates(rng);
  CoordinateType randomY = uniformCoordinates(rng);
  ValueType randomValue = uniformValues(rng);

  PointValue<CoordinateType, ValueType> randomPointValue(randomX, randomY,
                                                         randomValue);

  return randomPointValue;
}

static std::vector<PointValue<CoordinateType, ValueType>>
constructRandomPointValues(std::size_t minCoordinate, std::size_t maxCoordinate,
                           std::size_t minValue, std::size_t maxValue,
                           std::size_t count) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<CoordinateType> uniformCoordinates(
      minCoordinate, maxCoordinate);
  std::uniform_real_distribution<ValueType> uniformValues(minValue, maxValue);

  std::vector<PointValue<CoordinateType, ValueType>> randomPointValues;

  for (std::size_t i = 0; i < count; i++) {
    CoordinateType randomX = uniformCoordinates(rng);
    CoordinateType randomY = uniformCoordinates(rng);
    ValueType randomValue = uniformValues(rng);

    randomPointValues.emplace_back(randomX, randomY, randomValue);
  }

  return randomPointValues;
}

static PointTree<CoordinateType, ValueType>
constructRandomTree(std::size_t nodes) {
  auto randomPointValues = constructRandomPointValues(0, 100, 0, 1000, nodes);

  PointTree<CoordinateType, ValueType> randomTree(randomPointValues.begin(),
                                                  randomPointValues.end());

  return randomTree;
}

static void BM_Point_Tree_Init(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto randomValues =
        constructRandomPointValues(0, 100, 0, 100, state.range());
    state.ResumeTiming();

    PointTree<CoordinateType, ValueType> tree(randomValues.begin(),
                                              randomValues.end());
  }
}

static void BM_Point_Tree_Height(benchmark::State &state) {
  PointTree<CoordinateType, ValueType> tree;
  int64_t lastSize = 0;
  for (auto _ : state) {
    state.PauseTiming();
    if (lastSize != state.range()) {
      tree = constructRandomTree(state.range());
      lastSize = state.range();
    }
    state.ResumeTiming();

    auto result = tree.height();
  }
}

static void BM_Point_Tree_Nearest(benchmark::State &state) {
  PointTree<CoordinateType, ValueType> tree;
  int64_t lastSize = 0;
  for (auto _ : state) {
    state.PauseTiming();
    if (lastSize != state.range()) {
      tree = constructRandomTree(state.range());
      lastSize = state.range();
    }
    auto randomCoordinate = constructRandomCoordinates(0, 100);
    state.ResumeTiming();

    auto result = tree.nearest(randomCoordinate);
  }
}

static void BM_Point_Tree_Add_Without_Rebalance(benchmark::State &state) {
  PointTree<CoordinateType, ValueType> tree;
  for (auto _ : state) {
    state.PauseTiming();
    tree = constructRandomTree(state.range());
    auto randomCoordinatesValue = constructRandomPointValue(0, 100, 0, 100);
    state.ResumeTiming();

    tree.add(std::move(randomCoordinatesValue));
  }
}

static void BM_Point_Tree_Add_With_Rebalance(benchmark::State &state) {
  PointTree<CoordinateType, ValueType> tree;
  for (auto _ : state) {
    tree = constructRandomTree(state.range());
    auto randomCoordinatesValue = constructRandomPointValue(0, 100, 0, 100);
    state.ResumeTiming();

    tree.add(std::move(randomCoordinatesValue));
    tree.rebalance();
  }
}

static void BM_Point_Tree_Remove_With_Balance(benchmark::State &state) {
  PointTree<CoordinateType, ValueType> tree;
  for (auto _ : state) {
    state.PauseTiming();
    tree = constructRandomTree(state.range());
    auto randomCoordinatesValue = constructRandomPointValue(0, 100, 0, 100);
    tree.add(randomCoordinatesValue);
    tree.rebalance();
    state.ResumeTiming();

    tree.removeByPointValue(randomCoordinatesValue);
  }
}

static void BM_Point_Tree_Remove_Without_Balance(benchmark::State &state) {
  PointTree<CoordinateType, ValueType> tree;
  for (auto _ : state) {
    state.PauseTiming();
    tree = constructRandomTree(state.range());
    auto randomCoordinatesValue = constructRandomPointValue(0, 100, 0, 100);
    tree.add(randomCoordinatesValue);
    state.ResumeTiming();

    tree.removeByPointValue(randomCoordinatesValue);
  }
}

static void BM_Point_Tree_Range_With_Balance(benchmark::State &state) {
  PointTree<CoordinateType, ValueType> tree;
  for (auto _ : state) {
    state.PauseTiming();
    tree = constructRandomTree(state.range());
    auto randomCoordinates = constructRandomCoordinates(0, 100);
    const int range = 20;
    state.ResumeTiming();

    auto result = tree.range(randomCoordinates, range);
  }
}

static void BM_Point_Tree_Range_Without_Balance(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    PointTree<CoordinateType, ValueType> tree;
    int64_t size = state.range();
    for (int64_t i = 0; i < size; ++i) {
      auto randomCoordinatesValue = constructRandomPointValue(0, 100, 0, 100);
      tree.add(randomCoordinatesValue);
    }
    auto randomCoordinates = constructRandomCoordinates(0, 100);
    const int range = 20;
    state.ResumeTiming();

    auto result = tree.range(randomCoordinates, range);
  }
}

// static void BM_MultiKeyMatrix_Insert(benchmark::State &state) {
//   CoordinateMatrix<InnerMatrixType> matrix;
//   int64_t lastSize = 0;
//   for (auto _ : state) {
//     state.PauseTiming();
//     if (lastSize != state.range()) {
//       constructRandomMatrix(matrix, state.range());
//       lastSize = state.range();
//     }
//     Coordinates randomCoordinates =
//         constructRandomCoordinates(0, state.range());
//     InnerMatrixType randomValue = constructRandomValue<InnerMatrixType>();
//     state.ResumeTiming();

//     matrix.set(randomCoordinates, randomValue);
//   }
// }

// static void BM_MultiKeyMatrix_Remove(benchmark::State &state) {
//   CoordinateMatrix<InnerMatrixType> matrix;
//   int64_t lastSize = 0;
//   for (auto _ : state) {
//     state.PauseTiming();
//     if (lastSize != state.range()) {
//       constructRandomMatrix(matrix, state.range());
//       lastSize = state.range();
//     }
//     Coordinates randomCoordinates =
//         constructRandomCoordinates(0, state.range());
//     InnerMatrixType randomValue = constructRandomValue<InnerMatrixType>();
//     matrix.set(randomCoordinates, randomValue);
//     state.ResumeTiming();

//     bool result = matrix.remove(randomCoordinates, randomValue);

//     state.PauseTiming();
//     if (!result) {
//       state.SkipWithError("Remove operation did not indicate success");
//       return;
//     }
//     state.ResumeTiming();
//   }
// }

BENCHMARK(BM_Point_Tree_Init)->RangeMultiplier(2)->Range(2, 2 << 16);

// BENCHMARK(BM_Point_Tree_Height)->RangeMultiplier(2)->Range(2, 2 << 16);

// BENCHMARK(BM_Point_Tree_Nearest)->RangeMultiplier(2)->Range(2, 2 << 18);

// BENCHMARK(BM_Point_Tree_Add_Without_Rebalance)
//     ->RangeMultiplier(2)
//     ->Range(2, 2 << 18);

// BENCHMARK(BM_Point_Tree_Add_With_Rebalance)
//     ->RangeMultiplier(2)
//     ->Range(2, 2 << 18);

// BENCHMARK(BM_Point_Tree_Remove_Without_Balance)
//     ->RangeMultiplier(2)
//     ->Range(2, 2 << 18);

// BENCHMARK(BM_Point_Tree_Remove_With_Balance)
//     ->RangeMultiplier(2)
//     ->Range(2, 2 << 18);

// BENCHMARK(BM_Point_Tree_Range_Without_Balance)
//     ->RangeMultiplier(2)
//     ->Range(2, 2 << 18);

// BENCHMARK(BM_Point_Tree_Range_With_Balance)
//     ->RangeMultiplier(2)
//     ->Range(2, 2 << 18);

// BENCHMARK(BM_MultiKeyMatrix_Insert)
//     ->RangeMultiplier(2)
//     ->Range(2, 2 << 16)
//     ->Complexity();

// BENCHMARK(BM_MultiKeyMatrix_Remove)
//     ->RangeMultiplier(2)
//     ->Range(2, 2 << 16)
//     ->Complexity();

// // TODO: move, getByX, getByY

BENCHMARK_MAIN();