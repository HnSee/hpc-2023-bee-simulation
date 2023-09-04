cmake_minimum_required(VERSION 3.15)

project(
  bee-simulation
  VERSION 0.0.1
  LANGUAGES CXX)

option(PACKAGE_TESTS "Build the tests" ON)

set(SOURCES
    src/main.cpp
    src/agents/agent.cpp
    src/agents/agent.hpp
    src/agents/agent_type.cpp
    src/agents/agent_type.hpp
    src/agents/bee.cpp
    src/agents/bee.hpp
    src/agents/flower.cpp
    src/agents/flower.hpp
    src/agents/hive.cpp
    src/agents/hive.hpp
    src/extern/simplex/SimplexNoise.cpp
    src/extern/simplex/SimplexNoise.h
    src/world/chunking.cpp
    src/world/chunking.hpp
    src/world/generator.cpp
    src/world/generator.hpp
    src/world/seeding.cpp
    src/world/seeding.hpp
    src/world/worldstate.cpp
    src/world/worldstate.hpp
    src/utils/coordinates.hpp
    src/utils/ctd_array.hpp
    src/utils/pair_hash.hpp
    src/utils/point_tree.hpp)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)
find_package(spdlog REQUIRED)
find_package(cxxopts REQUIRED)
find_package(benchmark REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIROMM REQUIRED IMPORTED_TARGET cairomm-1.0)

if(PACKAGE_TESTS)
  include (CTest)
  enable_testing()
  find_package(GTest REQUIRED)

  macro(m_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} GTest::gtest_main GTest::gmock
                          spdlog::spdlog_header_only)
    gtest_discover_tests(${TESTNAME})
  endmacro()

  m_add_test(test_point_tree tests/test_point_tree.cpp)

  add_custom_target(
    test_memcheck
    COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --test-action memcheck
    COMMAND ${CMAKE_COMMAND} -E cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()

add_executable(bee-simulation ${SOURCES})
target_compile_features(bee-simulation PUBLIC cxx_std_14)

target_link_libraries(
  bee-simulation
    PUBLIC
      MPI::MPI_CXX
      OpenMP::OpenMP_CXX
      PkgConfig::CAIROMM
      spdlog::spdlog_header_only
      cxxopts::cxxopts)

install(TARGETS bee-simulation)
