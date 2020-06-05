#include "../../includes/View/Player.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Player::Player(const Renderer* renderer, int x_tile, int y_tile)
    : Entity(renderer, x_tile, y_tile) {}

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
                break;

            case SDLK_s:
                // mover un tile para abajo
                break;

            case SDLK_a:
                // mover un tile para la izquierda
                break;

            case SDLK_d:
                // mover un tile para la derecha
                break;
        }
    }
}

void Player::render() const {
    // Renderizar el personaje
    int x = x_tile * TILE_WIDTH;
    int y = y_tile * TILE_HEIGHT;

    // Centramos el x respecto del TILE_WIDTH
    x += (TILE_WIDTH - TILE_WIDTH) / 2;

    // Hacemos que los pies se grafiquen en el tail que le corresponde
    y += TILE_HEIGHT * (0.8);
    y -= TILE_HEIGHT;

    // Calculamos ahora el SDL_Rect que le corresponde
    SDL_Rect clip = {0, 0, PLAYER_SPRITE_W, PLAYER_SPRITE_H};
    SDL_Rect render_quad = {x, y, sprite_sheet.getWidth(),
                            sprite_sheet.getHeight()};
    g_renderer->renderIfVisible(sprite_sheet.getTexture(), &render_quad, &clip);
}

Player::~Player() {}

//-----------------------------------------------------------------------------
