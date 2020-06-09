#include "../../includes/View/Player.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Player::Player(const Renderer* renderer, const UnitSpriteContainer& sprites,
               const Predictor& predictor)
    : Unit(renderer, sprites), predictor(predictor) {}

void Player::handleEvent(const SDL_Event& e) {}

void Player::update(int corrected_x_tile, int corrected_y_tile) {
    /* Dependerá de si hacemos o no predictor de movimientos */
    x_tile = corrected_x_tile;
    y_tile = corrected_y_tile;
    _centerBoxOnTile();
}

void Player::act() {}

void Player::render() const {
    /*
    SDL_Rect render_quad = {box.x, box.y, sprite_sheet.getWidth(),
                            sprite_sheet.getHeight()};
    g_renderer->renderIfVisible(sprite_sheet.getTexture(), &render_quad,
                                &frame_clip);
                                */
}

SDL_Rect Player::getBox() const {
    return box;
}

SDL_Rect Player::getPos() const {
    return SDL_Rect({x_tile, y_tile, box.w, box.h});
}

Player::~Player() {}

//-----------------------------------------------------------------------------
