#include "../../includes/View/Entity.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Entity::_centerBoxOnTile() {
    box.x = (x_tile * TILE_WIDTH) + ((TILE_WIDTH - box.w) / 2);
    box.y = (y_tile * TILE_HEIGHT) + (TILE_HEIGHT * (0.8)) - box.h;
}

int Entity::_xValueToReach() const {
    return (x_tile * TILE_WIDTH) + ((TILE_WIDTH - box.w) / 2);
}

int Entity::_yValueToReach() const {
    return (y_tile * TILE_HEIGHT) + (TILE_HEIGHT * (0.8)) - box.h;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Entity::Entity(const Renderer* renderer)
    : g_renderer(renderer),
      x_tile(0),
      y_tile(0),
      box({0, 0, 0, 0}),
      x_vel(0),
      y_vel(0),
      next_x(0),
      next_y(0),
      last_moved(0) {}

void Entity::updatePosition(int corrected_x_tile, int corrected_y_tile) {
    x_tile = corrected_x_tile;
    y_tile = corrected_y_tile;
    // aca probablemente tenemos que poner a la entidad a moverse, poniendo
    // next_tiles en vez de xy_tiles
}

void Entity::move() {}

void Entity::loadMedia() {}

void Entity::handleEvent(const SDL_Event& e) {}

Entity::~Entity() {}

//-----------------------------------------------------------------------------
