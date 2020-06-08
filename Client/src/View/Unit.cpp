#include "../../includes/View/Unit.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Unit::_centerBoxOnTile() {
    box.x = (x_tile * TILE_WIDTH) + ((TILE_WIDTH - box.w) / 2);
    box.y = (y_tile * TILE_HEIGHT) + (TILE_HEIGHT * (0.8)) - box.h;
}

int Unit::_xValueToReach() const {
    return (x_tile * TILE_WIDTH) + ((TILE_WIDTH - box.w) / 2);
}

int Unit::_yValueToReach() const {
    return (y_tile * TILE_HEIGHT) + (TILE_HEIGHT * (0.8)) - box.h;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Unit::Unit(const Renderer* renderer, const int x_tile, const int y_tile)
    : g_renderer(renderer),
      x_tile(x_tile),
      y_tile(y_tile),
      box({0, 0, 0, 0}),
      x_vel(0),
      y_vel(0),
      next_x(0),
      next_y(0),
      last_moved(0) {}

void Unit::updatePosition(int corrected_x_tile, int corrected_y_tile) {
    x_tile = corrected_x_tile;
    y_tile = corrected_y_tile;
    // aca probablemente tenemos que poner a la entidad a moverse, poniendo
    // next_tiles en vez de xy_tiles
}

void Unit::move() {}

void Unit::loadMedia() {}

void Unit::handleEvent(const SDL_Event& e) {}

Unit::~Unit() {}

//-----------------------------------------------------------------------------
