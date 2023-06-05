#include <benchmark/benchmark.h>

#include "../src/utils/multikey_matrix.hpp"
#include "../src/utils/multikey_matrix_array.hpp"
#include <cstddef>
#include <random>

typedef unsigned char InnerMatrixType;

template <typename T>
static void constructRandomMatrix(MultiKeyMatrix<T> &matrix, std::size_t size) {
  matrix = MultiKeyMatrixArray<InnerMatrixType>(size);

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<T> uniform;

  for (std::size_t i = 0; i < size; i++) {
    for (std::size_t j = 0; j < size; j++) {
      MultiKeyIndex newIndex;
      newIndex.x = i;
      newIndex.y = j;

      matrix.set(newIndex, uniform(rng));
    }
  }
}

static MultiKeyIndex constructRandomIndex(std::size_t size) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<double> uniform(0, size);

  MultiKeyIndex randomIndex;
  randomIndex.x = uniform(rng);
  randomIndex.y = uniform(rng);
  return randomIndex;
}

static void BM_MultiKeyMatrix_Init_Array(benchmark::State &state) {
  for (auto _ : state) {
    MultiKeyMatrixArray<InnerMatrixType> matrix =
        MultiKeyMatrixArray<InnerMatrixType>(state.range());
  }
}

static void BM_MultiKeyMatrix_Get_Array(benchmark::State &state) {
  MultiKeyMatrixArray<InnerMatrixType> matrix(0);
  for (auto _ : state) {
    state.PauseTiming();
    constructRandomMatrix(matrix, state.range());
    MultiKeyIndex randomIndex = constructRandomIndex(state.range());
    state.ResumeTiming();

    InnerMatrixType *result = matrix.get(randomIndex);
  }
}

static void BM_MultiKeyMatrix_Map_Impl(benchmark::State &state) {
  std::string x = "hello";
  for (auto _ : state)
    std::string copy(x);
}

BENCHMARK(BM_MultiKeyMatrix_Init_Array)->RangeMultiplier(2)->Range(2, 2 << 10);
BENCHMARK(BM_MultiKeyMatrix_Get_Array)->RangeMultiplier(2)->Range(2, 2 << 10);
BENCHMARK(BM_MultiKeyMatrix_Map_Impl);

BENCHMARK_MAIN();