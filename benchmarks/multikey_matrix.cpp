#include <benchmark/benchmark.h>

#include "../src/utils/coordinate_matrix.hpp"
#include <cstddef>
#include <omp.h>
#include <random>

typedef unsigned char InnerMatrixType;

template <typename T>
static void constructRandomMatrix(CoordinateMatrix<T> &matrix,
                                  std::size_t size) {
  matrix = CoordinateMatrix<T>(size);

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<T> uniform;

  #pragma omp parallel for shared(matrix)
  for (std::size_t i = 0; i < size; i++) {
    for (std::size_t j = 0; j < size; j++) {
      Coordinates newCoordinates = std::make_pair(i, j);
      T randomValue = uniform(rng);
      {
      matrix.set(newCoordinates, randomValue);
      }
    }
  }
}

static Coordinates constructRandomCoordinates(std::size_t min,
                                              std::size_t max) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<double> uniform(min, max);

  Coordinates randomCoordinate(uniform(rng), uniform(rng));
  return randomCoordinate;
}

static void BM_MultiKeyMatrix_Init(benchmark::State &state) {
  for (auto _ : state) {
    CoordinateMatrix<InnerMatrixType> matrix =
        CoordinateMatrix<InnerMatrixType>(state.range());
  }
}

static void BM_MultiKeyMatrix_Get(benchmark::State &state) {
  CoordinateMatrix<InnerMatrixType> matrix;
  for (auto _InitInit : state) {
    state.PauseTiming();
    constructRandomMatrix(matrix, state.range());
    Coordinates randomCoordinates =
        constructRandomCoordinates(0, state.range());
    state.ResumeTiming();

    auto result = matrix.get(randomCoordinates);
  }
}

// BENCHMARK(BM_MultiKeyMatrix_Init)->RangeMultiplier(2)->Range(2, 2 << 16);
BENCHMARK(BM_MultiKeyMatrix_Get)->RangeMultiplier(2)->Range(2, 2 << 16)->Iterations(100);

BENCHMARK_MAIN();