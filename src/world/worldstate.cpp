#include "worldstate.hpp"

WorldState::WorldState(std::unique_ptr<WorldMap> map) {
    this->map = map;
    this->agents = std::make_unique();
}
