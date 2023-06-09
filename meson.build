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
  'src/world/worldstate.cpp',
  'src/agents/agent.cpp',
  'src/extern/simplex/SimplexNoise.cpp',
  'src/extern/kdtree/kdtree.cpp'
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

bm_worldmap = executable(
  'bm_worldmap',
  sources: [
    'benchmarks/worldmap.cpp'
  ],
  dependencies: [benchmarking_dep]
)

benchmark(
  'worldmap',
  bm_worldmap,
  verbose: true,
  timeout: 900,
  args: '--benchmark_out=results_worldmap.json'
)
