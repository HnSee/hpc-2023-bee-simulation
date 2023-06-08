project(
  'bee-simulation',
  'cpp',
  version: '0.0.1',
  default_options: [
    'warning_level=3',
    'cpp_std=c++14'
  ]
)

src = [
  'src/main.cpp',
  'src/world/generator.cpp',
  'src/extern/simplex/SimplexNoise.cpp'
]

spdlog = subproject('spdlog')
spdlog_dep = spdlog.get_variable('spdlog_dep')

cxxopts = subproject('cxxopts')
cxxopts_dep = cxxopts.get_variable('cxxopts_dep')

cairo_dep = dependency('cairomm-1.0')

benchmarking = subproject('google-benchmark')
benchmarking_dep = benchmarking.get_variable('google_benchmark_dep')

exe = executable(
  'bee_simulation',
  sources: src,
  dependencies: [spdlog_dep, cxxopts_dep, cairo_dep],
  install: true
)

bm_multikey_matrix = executable(
  'bm_multikey_matrix',
  sources: [
    'benchmarks/multikey_matrix.cpp'
  ],
  dependencies: [benchmarking_dep]
)

benchmark(
  'multikey_matrix',
  bm_multikey_matrix,
  verbose: true
)
