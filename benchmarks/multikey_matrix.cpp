#include <benchmark/benchmark.h>

#include "../src/utils/coordinate_matrix.hpp"
#include <cstddef>
#include <random>

typedef unsigned char InnerMatrixType;

static Coordinates constructRandomCoordinates(std::size_t min,
                                              std::size_t max) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<double> uniform(min, max);

  Coordinates randomCoordinate(uniform(rng), uniform(rng));
  return randomCoordinate;
}

template <typename T>
static void constructRandomMatrix(CoordinateMatrix<T> &matrix,
                                  std::size_t size) {
  matrix = CoordinateMatrix<T>(size);

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<T> uniform;

  for (std::size_t i = 0; i < size; i++) {
    Coordinates newCoordinates = constructRandomCoordinates(0, size);
    T randomValue = uniform(rng);
    matrix.set(newCoordinates, randomValue);
  }
}

static void BM_MultiKeyMatrix_Init(benchmark::State &state) {
  for (auto _ : state) {
    CoordinateMatrix<InnerMatrixType> matrix =
        CoordinateMatrix<InnerMatrixType>(state.range());
  }
}

static void BM_MultiKeyMatrix_Get(benchmark::State &state) {
  CoordinateMatrix<InnerMatrixType> matrix;
  std::size_t lastSize = 0;
  for (auto _InitInit : state) {
    state.PauseTiming();
    if (lastSize != state.range()) {
      constructRandomMatrix(matrix, state.range());
      lastSize = state.range();
    }
    Coordinates randomCoordinates =
        constructRandomCoordinates(0, state.range());
    state.ResumeTiming();

    auto result = matrix.get(randomCoordinates);
  }
}

BENCHMARK(BM_MultiKeyMatrix_Init)->RangeMultiplier(2)->Range(2, 2 << 16);
BENCHMARK(BM_MultiKeyMatrix_Get)->RangeMultiplier(2)->Range(2, 2 << 16)->Complexity();

// TODO: set, remove, getByX, getByY

BENCHMARK_MAIN();