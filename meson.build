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
  'src/agents/agent.cpp',
  'src/world/generator.cpp',
  'src/world/worldstate.cpp',
  'src/extern/simplex/SimplexNoise.cpp'
]

spdlog = subproject('spdlog')
spdlog_dep = spdlog.get_variable('spdlog_dep')

cxxopts = subproject('cxxopts')
cxxopts_dep = cxxopts.get_variable('cxxopts_dep')

cairo_dep = dependency('cairomm-1.0')

benchmarking = subproject('google-benchmark')
benchmarking_dep = benchmarking.get_variable('google_benchmark_dep')

gtest = subproject('gtest')
gtest_dep = gtest.get_variable('gtest_dep')
gtest_main_dep = gtest.get_variable('gtest_main_dep')

exe = executable(
  'bee_simulation',
  sources: src,
  dependencies: [spdlog_dep, cxxopts_dep, cairo_dep],
  install: true
)

test_two_d_tree = executable(
  'two_d_tree',
  sources: [
    'tests/test_two_d_tree.cpp'
  ],
  dependencies: [gtest_dep, gtest_main_dep]
)

test(
  'two_d_tree',
  test_two_d_tree,
  args: '--gtest_color=yes'
)

bm_two_d_tree = executable(
  'bm_two_d_tree',
  sources: [
    'benchmarks/two_d_tree.cpp'
  ],
  dependencies: [benchmarking_dep]
)

benchmark(
  'two_d_tree',
  bm_two_d_tree,
  verbose: true,
  timeout: 900,
  args: '--benchmark_out=results_twodmap.json'
)
