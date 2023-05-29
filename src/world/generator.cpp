#define JC_VORONOI_IMPLEMENTATION

#include "generator.hpp"
#include "../extern/jc_voronoi.h"
#include <cairomm/context.h>
#include <cairomm/surface.h>
#include <cstring>
#include <spdlog/spdlog.h>
#include <sstream>

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

WorldGenerator::WorldGenerator(unsigned int seed, unsigned int edgeLength,
                               unsigned int biomes, unsigned int relaxations) {
  this->seed = seed;
  this->edgeLength = edgeLength;
  this->biomes = biomes;
  this->relaxations = relaxations;
  currentWorld = new worldCell[edgeLength * edgeLength];
}

WorldGenerator::~WorldGenerator() { delete[] currentWorld; }

worldCell *WorldGenerator::generateWorld() {
  srand(this->seed);
  memset(&this->currentVoronoiRepresentation, 0, sizeof(jcv_diagram));

  jcv_rect boundingBox;
  boundingBox.min.x = 0;
  boundingBox.min.y = 0;
  boundingBox.max.x = this->edgeLength;
  boundingBox.max.y = this->edgeLength;

  jcv_point *points = new jcv_point[this->biomes]();

  spdlog::debug("Generating random biome points...");
  for (unsigned int b = 0; b < this->biomes; b++) {
    points[b].x = static_cast<float>(rand()) /
                  (static_cast<float>(RAND_MAX / this->edgeLength));
    points[b].y = static_cast<float>(rand()) /
                  (static_cast<float>(RAND_MAX / this->edgeLength));
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

  return this->currentWorld;
}

void WorldGenerator::generateWorldImage(std::string outputPath) {
  unsigned int width = this->edgeLength;

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
    cr->arc(site->p.x, site->p.y, 50, 0, 2 * M_PI);
    cr->fill();
  }

  int dimensions[] = {100, 10, 1};
  int dimensions_num = 3;
  int dimensions_distance = 5;
  int dimensions_line_width = 5;
  int dimensions_text_distance = 2;
  std::string unit = "dm";

  for (int i = 0; i < dimensions_num; i++) {
    cr->move_to(0.01 * width, 0.01 * width + i * dimensions_distance);
    cr->set_source_rgb(0, 0, 0);
    cr->rel_line_to(dimensions[i], 0);
    cr->set_line_width(dimensions_line_width);
    cr->stroke();

    cr->move_to(0.01 * width, 0.01 * width + i * dimensions_distance);
    cr->rel_move_to(dimensions[i] / 2, dimensions_text_distance);
    std::ostringstream unitText;
    unitText << dimensions[i] << " " << unit;
    cr->set_font_size(10);
    cr->show_text(unitText.str());
  }

  cr->show_page();
}
