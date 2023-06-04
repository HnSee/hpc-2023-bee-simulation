#define JC_VORONOI_IMPLEMENTATION
#define DB_PERLIN_IMPL

#include "generator.hpp"
#include "../extern/jc_voronoi.h"
#include "../extern/simplex/SimplexNoise.h"
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <cstring>
#include <random>
#include <spdlog/spdlog.h>
#include <sstream>
#include <unordered_map>

float clip(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

static void relax_points(const jcv_diagram *diagram, jcv_point *points) {
  const jcv_site *sites = jcv_diagram_get_sites(diagram);
  for (int i = 0; i < diagram->numsites; ++i) {
    const jcv_site *site = &sites[i];
    jcv_point sum = site->p;
    int count = 1;

    const jcv_graphedge *edge = site->edges;

    while (edge) {
      sum.x += edge->pos[0].x;
      sum.y += edge->pos[0].y;
      ++count;
      edge = edge->next;
    }

    points[site->index].x = sum.x / (jcv_real)count;
    points[site->index].y = sum.y / (jcv_real)count;
  }
}

WorldMap *WorldGenerator::generateWorld() {
  this->currentZoom = this->initialZoom;
  srand(this->seed);

  this->generateVoronoiRepresentation();
  this->generateVoronoiSVG("stage_1.svg");
  this->rasterizeVoronoiRepresentation();
  this->generateWorldImage("stage_3.png");
  this->blurEdges();
  this->generateWorldImage("stage_4.png");
  this->assignBiomes();
  this->generateWorldImageWithBiomeColor("stage_5.png");

  return &this->currentWorld;
}

void WorldGenerator::generateVoronoiRepresentation() {
  memset(&this->currentVoronoiRepresentation, 0, sizeof(jcv_diagram));

  jcv_rect boundingBox;
  boundingBox.min.x = 0;
  boundingBox.min.y = 0;
  boundingBox.max.x = this->size;
  boundingBox.max.y = this->size;

  jcv_point *points = new jcv_point[this->biomes]();

  spdlog::debug("Generating random biome points...");
  for (unsigned int b = 0; b < this->biomes; b++) {
    points[b].x = static_cast<float>(rand()) /
                  (static_cast<float>(RAND_MAX / this->size));
    points[b].y = static_cast<float>(rand()) /
                  (static_cast<float>(RAND_MAX / this->size));
  }
  spdlog::debug("Random biome points generated.");

  spdlog::debug("Performing relaxation of the points...");
  for (int i = 0; i < this->relaxations; i++) {
    memset(&this->currentVoronoiRepresentation, 0, sizeof(jcv_diagram));
    jcv_diagram_generate(this->biomes, points, &boundingBox, nullptr,
                         &this->currentVoronoiRepresentation);
    relax_points(&this->currentVoronoiRepresentation, points);
    jcv_diagram_free(&this->currentVoronoiRepresentation);
  }
  spdlog::debug("Relaxation succeeded.");

  spdlog::debug("Generating voronoi diagram...");
  memset(&this->currentVoronoiRepresentation, 0, sizeof(jcv_diagram));
  jcv_diagram_generate(this->biomes, points, &boundingBox, nullptr,
                       &this->currentVoronoiRepresentation);
  spdlog::debug("Voronoi diagram generated.");
}

void WorldGenerator::generateVoronoiSVG(std::string outputPath) {
  unsigned int width = this->size;

  auto surface = Cairo::SvgSurface::create(outputPath, width, width);
  auto cr = Cairo::Context::create(surface);

  const jcv_site *sites =
      jcv_diagram_get_sites(&this->currentVoronoiRepresentation);

  for (int s = 0; s < this->currentVoronoiRepresentation.numsites; s++) {
    const jcv_site *site = &sites[s];

    double randomColor[3];
    randomColor[0] =
        static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    randomColor[1] =
        static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    randomColor[2] =
        static_cast<double>(rand()) / static_cast<double>(RAND_MAX);

    cr->save();
    cr->set_source_rgb(randomColor[0], randomColor[1], randomColor[2]);
    const jcv_graphedge *e = site->edges;
    cr->move_to(e->pos[0].x, e->pos[0].y);
    e = e->next;

    while (e) {
      cr->line_to(e->pos[0].x, e->pos[0].y);
      e = e->next;
    }

    cr->close_path();
    cr->fill();

    cr->set_source_rgb(255, 0, 0);
    cr->arc(site->p.x, site->p.y, 10, 0, 2 * M_PI);
    cr->fill();
  }

  cr->show_page();
}

void WorldGenerator::rasterizeVoronoiRepresentation() {
  unsigned int width = this->size;

  auto surface =
      Cairo::ImageSurface::create(Cairo::Format::FORMAT_RGB24, width, width);
  auto cr = Cairo::Context::create(surface);

  const jcv_site *sites =
      jcv_diagram_get_sites(&this->currentVoronoiRepresentation);

  double colorMax = this->currentVoronoiRepresentation.numsites;

  for (int s = 0; s < this->currentVoronoiRepresentation.numsites; s++) {
    const jcv_site *site = &sites[s];

    cr->set_source_rgb(s / colorMax, s / colorMax, s / colorMax);
    cr->set_antialias(Cairo::Antialias::ANTIALIAS_NONE);
    cr->set_line_width(0);

    const jcv_graphedge *e = site->edges;
    cr->move_to(e->pos[0].x, e->pos[0].y);
    e = e->next;

    while (e) {
      cr->line_to(e->pos[0].x, e->pos[0].y);
      e = e->next;
    }

    cr->close_path();
    cr->fill();
  }

  // auto nearestFilter = Cairo::SurfacePattern::create(surface);
  // nearestFilter->set_filter(Cairo::Filter::FILTER_BEST);
  // cr->set_source(nearestFilter);
  // cr->mask(nearestFilter);

  surface->write_to_png("stage_2.png");

  auto byteData = surface->get_data();
  int maxCells = surface->get_width() * surface->get_height() * 4;
  int stride = surface->get_stride();

  int by = 0;
  int row = 0;

  this->currentWorld = std::vector<std::vector<WorldCell>>(
      this->size, std::vector<WorldCell>(this->size));

  while (by < maxCells) {
    int currentX = (by - row * stride) / 4;
    int currentY = row;

    this->currentWorld[currentX][currentY] = byteData[by];

    by += 4;

    if (by % stride == 0) {
      row++;
    }
  }
}

void WorldGenerator::blurEdges() {
  WorldMap newMap(this->size, std::vector<WorldCell>(this->size));

  std::vector<std::vector<std::pair<double, double>>> noiseMap(
      this->size, std::vector<std::pair<double, double>>(this->size));

  std::vector<std::vector<std::pair<int, int>>> displacementMap(
      this->size, std::vector<std::pair<int, int>>(this->size));

  SimplexNoise noiseGenerator = SimplexNoise();
  double resolution = 32;
  double scale = (double)this->size / resolution;

  for (std::size_t i = 0; i < displacementMap.size(); i++) {
    for (std::size_t j = 0; j < displacementMap[i].size(); j++) {
      displacementMap[i][j] = std::pair<int, int>(
          clip(i + this->edgeDisplacement *
                       noiseGenerator.fractal(this->perlinOctaves,
                                              (i + 0.1) / scale, j / scale, 1),
               0, this->size - 1),
          clip(j + this->edgeDisplacement *
                       noiseGenerator.fractal(this->perlinOctaves,
                                              (i + 0.1) / scale, j / scale,
                                              0.5),
               0, this->size - 1));
    }
  }

  for (std::size_t x = 0; x < displacementMap.size(); x++) {
    for (std::size_t y = 0; y < displacementMap[x].size(); y++) {
      newMap[x][y] = this->currentWorld[displacementMap[x][y].first]
                                       [displacementMap[x][y].second];
    }
  }

  this->currentWorld = std::move(newMap);
}

void WorldGenerator::assignBiomes() {
  std::unordered_map<unsigned char, Biome> biomeMap;

  std::size_t biomeCount = this->biomeProbabilities.size();
  std::vector<Biome> biomeDistributionKeys;
  std::vector<float> biomeDistributionProbability;

  for (auto const &bd : this->biomeProbabilities) {
    biomeDistributionKeys.push_back(bd.first);
    biomeDistributionProbability.push_back(bd.second);
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::discrete_distribution<> biomeDistribution(
      biomeDistributionProbability.begin(), biomeDistributionProbability.end());

  // Collect biome representations
  for (std::size_t x = 0; x < this->size; x++) {
    for (std::size_t y = 0; y < this->size; y++) {
      unsigned char cellValue = this->currentWorld[x][y];
      auto biome = biomeMap.insert(std::pair<unsigned char, Biome>(
          cellValue, Biome(biomeDistribution(gen) % biomeCount)));
      this->currentWorld[x][y] = (WorldCell)biome.first->second;
    }
  }
}

void WorldGenerator::generateWorldImage(std::string outputPath) {
  unsigned int width = this->size;

  auto surface =
      Cairo::ImageSurface::create(Cairo::Format::FORMAT_RGB24, width, width);
  auto cr = Cairo::Context::create(surface);

  auto rows = this->currentWorld.size();
  auto cols = rows;

  for (std::size_t row = 0; row < rows; row++) {
    for (std::size_t col = 0; col < cols; col++) {
      double value = (double)this->currentWorld[row][col] / 255.0;
      cr->set_source_rgb(value, value, value);
      cr->rectangle(row, col, 1, 1);
      cr->fill();
    }
  }

  surface->write_to_png(outputPath);
}

void WorldGenerator::generateWorldImageWithBiomeColor(std::string outputPath) {
  unsigned int width = this->size;

  auto surface =
      Cairo::ImageSurface::create(Cairo::Format::FORMAT_RGB24, width, width);
  auto cr = Cairo::Context::create(surface);

  auto rows = this->currentWorld.size();
  auto cols = rows;

  for (std::size_t row = 0; row < rows; row++) {
    for (std::size_t col = 0; col < cols; col++) {
      Biome value = Biome(this->currentWorld[row][col]);
      Color color = this->biomeColors[value];
      cr->set_source_rgb(color[0], color[1], color[2]);
      cr->rectangle(row, col, 1, 1);
      cr->fill();
    }
  }

  surface->write_to_png(outputPath);
}
