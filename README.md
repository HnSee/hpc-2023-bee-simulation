# Simulation of a Simplified Ecosystem to Study the Influence of Environmental Factors on Bee Populations

Project for the **Practical Course on High-Performance Computing** at the Georg-August-Universität Göttingen, summer term 2023.

## Idea
The main idea of the project is to implement a simulation for studying bee behavior in a simplified ecosystem. Our motivation is the bee mortality observed over the last decades.

## Dependencies
- A C++14 compatible C++ compiler like [GCC](https://gcc.gnu.org/) [[packages](https://repology.org/project/gcc/versions)]
- [Meson](https://mesonbuild.com/) *or* [CMake](https://cmake.org/) (build system) [[packages](https://repology.org/project/cmake/versions)]
- [Ninja](https://repology.org/project/ninja/versions) (build system backend, [others possible](https://mesonbuild.com/Running-Meson.html)) [[packages](https://repology.org/project/ninja/versions)]
- [Cairo](https://www.cairographics.org/)/[cairomm](https://www.cairographics.org/cairomm/) (graphics library used in world generation steps) [[packages](https://repology.org/project/cairomm/versions)]
- [spdlog](https://github.com/gabime/spdlog) (logging) [[packages](https://repology.org/project/spdlog/versions)]
- [cxxopts](https://github.com/jarro2783/cxxopts) (command line arguments parsing) [[packages][https://repology.org/project/cxxopts/versions]]
- [Benchmark](https://github.com/google/benchmark) (benchmarking) [[packages](https://repology.org/project/benchmark/versions)]
- [GoogleTest](https://github.com/google/googletest) (testing) [[packages](https://repology.org/project/gtest/versions)]

Some simple libraries or implementations have been transferred directly into this repository and can be found under `src/extern/`:
- [jc_voronoi](https://github.com/JCash/voronoi) (Fortune's sweep algorithm implementation used in world generation steps)
- [Perlin Simplex Noise C++ Implementation](https://github.com/SRombauts/SimplexNoise) (Perlin's Simplex noise implementation used in world generation steps)

## Building
Building the application uses [Meson](https://mesonbuild.com/) (or [CMake](https://cmake.org/)) and is therefore very simple. The process consists of two steps.

First, configure a building directory `builddir`:
```console
meson setup builddir
```
Note that `builddir` may be any name you want.

Then, compile the application:
```console
meson compile -C builddir
```
The binary can then be found under `builddir/bee_simulation`.

To compile the application again after some changes have been made, only the second step is necessary. Meson/Ninja will only recompile necessary components of the application.

## Tests
Tests can be compiled and run using:
```console
meson test -C builddir --verbose bee-simulation:
```
(The `bee-simulation:` arguments tells Meson to only run our own tests instead of the third-party libraries' tests.)

## Benchmarks
To get an overview of the performance of some application components like custom data structures, some benchmarks have been created using Google's [Benchmark](https://github.com/google/benchmark) library. These benchmarks are not supposed to enable a precise performance analysis.

They can be compiled and run using:
```console
meson test -C builddir --verbose --benchmark
```
