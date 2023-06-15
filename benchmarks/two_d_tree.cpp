#include "../src/utils/two_d_tree.hpp"
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

static CoordinatesValue<CoordinateType, ValueType>
constructRandomCoordinatesValue(std::size_t minCoordinate,
                                std::size_t maxCoordinate, std::size_t minValue,
                                std::size_t maxValue) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<CoordinateType> uniformCoordinates(
      minCoordinate, maxCoordinate);
  std::uniform_real_distribution<ValueType> uniformValues(minValue, maxValue);

  std::vector<CoordinatesValue<CoordinateType, ValueType>>
      randomCoordinatesValues;

  CoordinateType randomX = uniformCoordinates(rng);
  CoordinateType randomY = uniformCoordinates(rng);
  ValueType randomValue = uniformValues(rng);

  CoordinatesValue<CoordinateType, ValueType> randomCoordinatesValue(
      randomX, randomY, randomValue);

  return randomCoordinatesValue;
}

static std::vector<CoordinatesValue<CoordinateType, ValueType>>
constructRandomCoordinatesValues(std::size_t minCoordinate,
                                 std::size_t maxCoordinate,
                                 std::size_t minValue, std::size_t maxValue,
                                 std::size_t count) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<CoordinateType> uniformCoordinates(
      minCoordinate, maxCoordinate);
  std::uniform_real_distribution<ValueType> uniformValues(minValue, maxValue);

  std::vector<CoordinatesValue<CoordinateType, ValueType>>
      randomCoordinatesValues;

  for (std::size_t i = 0; i < count; i++) {
    CoordinateType randomX = uniformCoordinates(rng);
    CoordinateType randomY = uniformCoordinates(rng);
    ValueType randomValue = uniformValues(rng);

    CoordinatesValue<CoordinateType, ValueType> randomCoordinatesValue(
        randomX, randomY, randomValue);

    randomCoordinatesValues.push_back(std::move(randomCoordinatesValue));
  }

  return randomCoordinatesValues;
}

static TwoDTree<CoordinateType, ValueType>
constructRandomTree(std::size_t nodes) {
  auto randomCoordinatesValues =
      constructRandomCoordinatesValues(0, 100, 0, 1000, nodes);

  TwoDTree<CoordinateType, ValueType> randomTree(
      randomCoordinatesValues.begin(), randomCoordinatesValues.end());

  return randomTree;
}

// template <typename T>
// static void constructRandomMatrix(CoordinateMatrix<T> &matrix,
//                                   std::size_t size) {
//   matrix = CoordinateMatrix<T>(size);

//   std::random_device dev;
//   std::mt19937 rng(dev());
//   std::uniform_int_distribution<T> uniform;

//   for (std::size_t i = 0; i < size; i++) {
//     Coordinates newCoordinates = constructRandomCoordinates(0, size);
//     T randomValue = uniform(rng);
//     matrix.set(newCoordinates, randomValue);
//   }
// }

// template <typename T> static T constructRandomValue() {
//   std::random_device dev;
//   std::mt19937 rng(dev());
//   std::uniform_int_distribution<T> uniform;

//   return uniform(rng);
// }

static void BM_Two_D_Tree_Init(benchmark::State &state) {
  for (auto _ : state) {
    state.PauseTiming();
    auto randomValues =
        constructRandomCoordinatesValues(0, 100, 0, 100, state.range());
    state.ResumeTiming();

    TwoDTree<CoordinateType, ValueType> tree(randomValues.begin(),
                                             randomValues.end());
  }
}

static void BM_Two_D_Tree_Height(benchmark::State &state) {
  TwoDTree<CoordinateType, ValueType> tree;
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

static void BM_Two_D_Tree_Nearest(benchmark::State &state) {
  TwoDTree<CoordinateType, ValueType> tree;
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

static void BM_Two_D_Tree_Add_Single(benchmark::State &state) {
  TwoDTree<CoordinateType, ValueType> tree;
  int64_t lastSize = 0;
  for (auto _ : state) {
    state.PauseTiming();
    if (lastSize != state.range()) {
      tree = constructRandomTree(state.range());
      lastSize = state.range();
    }
    auto randomCoordinatesValue =
        constructRandomCoordinatesValue(0, 100, 0, 100);
    state.ResumeTiming();

    tree.add(std::move(randomCoordinatesValue));
  }
}

static void BM_Two_D_Tree_Add_Range(benchmark::State &state) {
  TwoDTree<CoordinateType, ValueType> tree;
  int64_t lastSize = 0;
  for (auto _ : state) {
    state.PauseTiming();
    if (lastSize != state.range()) {
      tree = constructRandomTree(state.range(0));
      lastSize = state.range();
    }
    auto randomCoordinatesValues =
        constructRandomCoordinatesValues(0, 100, 0, 100, state.range(1));
    state.ResumeTiming();

    tree.addRange(randomCoordinatesValues.begin(),
                  randomCoordinatesValues.end());
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

// BENCHMARK(BM_Two_D_Tree_Init)->RangeMultiplier(2)->Range(2, 2 << 16);

// BENCHMARK(BM_Two_D_Tree_Height)->RangeMultiplier(2)->Range(2, 2 << 16);

// BENCHMARK(BM_Two_D_Tree_Nearest)->RangeMultiplier(2)->Range(2, 2 << 18);

BENCHMARK(BM_Two_D_Tree_Add_Single)->RangeMultiplier(2)->Range(2, 2 << 18);

BENCHMARK(BM_Two_D_Tree_Add_Range)
    ->RangeMultiplier(2)
    ->Ranges({{2, 2 << 18}, {2, 2 << 18}});

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