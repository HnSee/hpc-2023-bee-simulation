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
  'src/agents/agent_type.cpp',
  'src/agents/bee.cpp',
  'src/agents/flower.cpp',
  'src/agents/hive.cpp',
  'src/world/chunking.cpp',
  'src/world/generator.cpp',
  'src/world/seeding.cpp',
  'src/world/worldstate.cpp',
  'src/extern/simplex/SimplexNoise.cpp'
]

mpi_dep = dependency('mpi', language: 'cpp')
omp_dep = dependency('openmp', language: 'cpp')
cairo_dep = dependency('cairomm-1.0')
spdlog_dep = dependency('spdlog')
cxxopts_dep = dependency('cxxopts')
benchmark_dep = dependency('benchmark')
gtest_dep = dependency('gtest', main: true)
gmock_dep = dependency('gmock')

exe = executable(
  'bee_simulation',
  sources: src,
  dependencies: [spdlog_dep, cxxopts_dep, cairo_dep, mpi_dep, omp_dep],
  install: true
)

test_point_tree = executable(
  'test_point_tree',
  sources: [
    'tests/test_point_tree.cpp'
  ],
  dependencies: [gtest_dep, spdlog_dep, gmock_dep]
)

test_ctd_array = executable(
  'test_ctd_array',
  sources: [
    'tests/test_ctd_array.cpp'
  ],
  dependencies: [gtest_dep]
)

test_chunking = executable(
  'test_chunking',
  sources: [
    'src/world/chunking.cpp',
    'tests/test_chunking.cpp'
  ],
  dependencies: [gtest_dep]
)

test_agents = executable(
  'test_agents',
  sources: [
    'src/agents/agent.cpp',
    'src/agents/bee.cpp',
    'src/agents/flower.cpp',
    'src/agents/hive.cpp',
    'src/world/chunking.cpp',
    'src/world/generator.cpp',
    'src/world/worldstate.cpp',
    'src/extern/simplex/SimplexNoise.cpp',
    'tests/test_agents.cpp',
    'src/agents/agent_type.cpp'
  ],
  dependencies: [gtest_dep, cairo_dep, spdlog_dep, omp_dep]
)

test_worldstate = executable(
  'test_worldstate',
  sources: [
    'src/agents/agent.cpp',
    'src/agents/agent_type.cpp',
    'src/agents/bee.cpp',
    'src/agents/flower.cpp',
    'src/agents/hive.cpp',
    'src/world/chunking.cpp',
    'src/world/worldstate.cpp',
    'tests/test_worldstate.cpp',
  ],
  dependencies: [gtest_dep, spdlog_dep]
)

test(
  'point_tree',
  test_point_tree,
  args: '--gtest_color=yes'
)

test(
  'ctd_array',
  test_ctd_array,
  args: '--gtest_color=yes'
)

test(
  'chunking',
  test_chunking,
  args: '--gtest_color=yes'
)

test(
  'agents',
  test_agents,
  args: '--gtest_color=yes',
  timeout: 4000
)

test(
  'worldstate',
  test_worldstate,
  args: '--gtest_color=yes'
)

bm_point_tree = executable(
  'bm_point_tree',
  sources: [
    'benchmarks/point_tree.cpp'
  ],
  dependencies: [benchmark_dep, spdlog_dep]
)

benchmark(
  'point_tree',
  bm_point_tree,
  verbose: true,
  timeout: 999999999999,
  args: '--benchmark_out=results_pointtree.json'
)
