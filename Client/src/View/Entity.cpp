#include "../../includes/View/Entity.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Entity::_centerBoxOnTile() {
    box.x = (x_tile * TILE_WIDTH) + ((TILE_WIDTH - box.w) / 2);
    box.y = (y_tile * TILE_HEIGHT) + (TILE_HEIGHT * (0.8)) - box.h;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Entity::Entity(const Renderer* renderer)
    : g_renderer(renderer), x_tile(0), y_tile(0), box({0, 0, 0, 0}) {}

void Entity::correctPosition(int corrected_x_tile, int corrected_y_tile) {
    x_tile = corrected_x_tile;
    y_tile = corrected_y_tile;
}

void Entity::loadMedia() {}

void Entity::handleEvent(const SDL_Event& e) {}

Entity::~Entity() {}

//-----------------------------------------------------------------------------
