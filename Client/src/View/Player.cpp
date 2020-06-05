#include "../../includes/View/Player.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Player::Player(const Renderer* renderer, int x_tile, int y_tile)
    : Entity(renderer) {
    this->x_tile = x_tile;
    this->y_tile = y_tile;
    box.w = PLAYER_SPRITE_W;
    box.h = PLAYER_SPRITE_H;
    _centerBoxOnTile();
}

void Player::loadMedia() {
    sprite_sheet.loadFromFile(
        g_renderer, SPRITES_PATH + std::string("4006.png"), PLAYER_SPRITE_BG_R,
        PLAYER_SPRITE_BG_G, PLAYER_SPRITE_BG_B);
}

void Player::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_w:
                // mover un tile para arriba
                y_tile--;
                break;

            case SDLK_s:
                // mover un tile para abajo
                y_tile++;
                break;

            case SDLK_a:
                // mover un tile para la izquierda
                x_tile--;
                break;

            case SDLK_d:
                // mover un tile para la derecha
                x_tile++;
                break;
        }
    }
}

void Player::move() {
    _centerBoxOnTile();
}

void Player::render() const {
    // Calculamos el SDL_Rect que le corresponde
    SDL_Rect clip = {0, 0, PLAYER_SPRITE_W, PLAYER_SPRITE_H};

    SDL_Rect render_quad = {box.x, box.y, sprite_sheet.getWidth(),
                            sprite_sheet.getHeight()};
    g_renderer->renderIfVisible(sprite_sheet.getTexture(), &render_quad, &clip);
}

const SDL_Rect* Player::getBox() const {
    return &box;
}

Player::~Player() {}

//-----------------------------------------------------------------------------
