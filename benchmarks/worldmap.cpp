// #include <benchmark/benchmark.h>

// #include <cstddef>
// #include <random>

// typedef unsigned char InnerMatrixType;

// static Coordinates constructRandomCoordinates(std::size_t min,
//                                               std::size_t max) {
//   std::random_device dev;
//   std::mt19937 rng(dev());
//   std::uniform_real_distribution<double> uniform(min, max);

//   Coordinates randomCoordinate(uniform(rng), uniform(rng));
//   return randomCoordinate;
// }

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

// static void BM_MultiKeyMatrix_Init(benchmark::State &state) {
//   for (auto _ : state) {
//     CoordinateMatrix<InnerMatrixType> matrix =
//         CoordinateMatrix<InnerMatrixType>(state.range());
//   }
// }

// static void BM_MultiKeyMatrix_Get(benchmark::State &state) {
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
//     state.ResumeTiming();

//     auto result = matrix.get(randomCoordinates);
//   }
// }

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

// BENCHMARK(BM_MultiKeyMatrix_Init)->RangeMultiplier(2)->Range(2, 2 << 16);

// BENCHMARK(BM_MultiKeyMatrix_Get)
//     ->RangeMultiplier(2)
//     ->Range(2, 2 << 16)
//     ->Complexity();

// BENCHMARK(BM_MultiKeyMatrix_Insert)
//     ->RangeMultiplier(2)
//     ->Range(2, 2 << 16)
//     ->Complexity();

// BENCHMARK(BM_MultiKeyMatrix_Remove)
//     ->RangeMultiplier(2)
//     ->Range(2, 2 << 16)
//     ->Complexity();

// // TODO: move, getByX, getByY

// BENCHMARK_MAIN();