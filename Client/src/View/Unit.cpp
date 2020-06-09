#include "../../includes/View/Unit.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Unit::_centerBoxOnTile() {
    box.x = (x_tile * tile_w) + ((tile_w - box.w) / 2);
    box.y = (y_tile * tile_h) + (tile_h * (0.8)) - box.h;
}

int Unit::_xValueToReach() const {
    return (x_tile * tile_w) + ((tile_w - box.w) / 2);
}

int Unit::_yValueToReach() const {
    return (y_tile * tile_h) + (tile_h * (0.8)) - box.h;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Unit::Unit(const Renderer* renderer, const UnitSpriteContainer& sprites)
    : g_renderer(renderer),
      g_sprites(sprites),
      tile_w(0),
      tile_h(0),
      x_tile(0),
      y_tile(0),
      box({0, 0, 0, 0}),
      x_vel(0),
      y_vel(0),
      next_x(0),
      next_y(0),
      last_moved(0) {}

void Unit::init(const int tile_w, const int tile_h) {
    this->tile_w = tile_w;
    this->tile_h = tile_h;
}

void Unit::update(int corrected_x_tile, int corrected_y_tile) {
    /* Actualizar información según el servidor */
    /* Si es necesario, iniciar movimiento*/
}

void Unit::act() {}

void Unit::render() const {}

Unit::~Unit() {}

//-----------------------------------------------------------------------------
