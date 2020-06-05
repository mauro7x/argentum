#include "../../includes/View/Player.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Player::Player(SDL_Renderer* renderer, int x_tile, int y_tile)
    : Entity(renderer, x_tile, y_tile) {}

void Player::loadMedia() {}

void Player::render() const {}

Player::~Player() {}

//-----------------------------------------------------------------------------
