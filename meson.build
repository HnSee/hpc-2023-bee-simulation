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
  'src/world/generator.cpp'
]

spdlog = subproject('spdlog')
spdlog_dep = spdlog.get_variable('spdlog_dep')

cxxopts = subproject('cxxopts')
cxxopts_dep = cxxopts.get_variable('cxxopts_dep')

cairo_dep = dependency('cairomm-1.0')

exe = executable(
  'bee_simulation',
  sources: src,
  dependencies: [spdlog_dep, cxxopts_dep, cairo_dep],
  install: true
)

# test('basic', exe)
