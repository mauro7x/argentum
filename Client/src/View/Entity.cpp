#include "../../includes/View/Entity.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Entity::Entity(const Renderer* renderer, int x_tile, int y_tile)
    : g_renderer(renderer), x_tile(x_tile), y_tile(y_tile) {}

void Entity::correctPosition(int corrected_x_tile, int corrected_y_tile) {
    x_tile = corrected_x_tile;
    y_tile = corrected_y_tile;
}

void Entity::loadMedia() {}

void Entity::handleEvent(const SDL_Event& e) {}

Entity::~Entity() {}

//-----------------------------------------------------------------------------
