#include "../../includes/View/Tile.h"

//-----------------------------------------------------------------------------
// Métodos privados

bool Tile::_checkCollision(const SDL_Rect& box) const {
    int left_me, left_box;
    int right_me, right_box;
    int top_me, top_box;
    int bottom_me, bottom_box;

    // Calculate the sides of rect A
    left_me = dim.x;
    right_me = dim.x + dim.w;
    top_me = dim.y;
    bottom_me = dim.y + dim.h;

    // Calculate the sides of rect B
    left_box = box.x;
    right_box = box.x + box.w;
    top_box = box.y;
    bottom_box = box.y + box.h;

    // If any of the sides from A are outside of B
    if (bottom_me <= top_box) {
        return false;
    }

    if (top_me >= bottom_box) {
        return false;
    }

    if (right_me <= left_box) {
        return false;
    }

    if (left_me >= right_box) {
        return false;
    }

    // If none of the sides from A are outside B
    return true;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Tile::Tile(int x, int y, Texture* texture, SDL_Renderer* renderer)
    : g_texture(texture), g_renderer(renderer) {
    dim.x = x;
    dim.y = y;
    dim.w = TILE_WIDTH;
    dim.h = TILE_HEIGHT;
}

Tile::Tile(Tile&& other) {
    this->g_texture = std::move(other.g_texture);
    this->g_renderer = std::move(other.g_renderer);
    this->dim = std::move(other.dim);
    other.g_texture = NULL;
    other.g_renderer = NULL;
}

Tile& Tile::operator=(Tile&& other) {
    this->g_texture = std::move(other.g_texture);
    this->g_renderer = std::move(other.g_renderer);
    this->dim = std::move(other.dim);
    other.g_texture = NULL;
    other.g_renderer = NULL;
    return *this;
}

void Tile::render(const SDL_Rect& camera) const {
    if (_checkCollision(camera)) {
        SDL_Rect scale = {0, 0, TILE_WIDTH, TILE_HEIGHT};
        g_texture->render(dim.x - camera.x + SCREEN_X_OFFSET,
                          dim.y - camera.y + SCREEN_Y_OFFSET, NULL, &scale);

        /* Implementación vieja para sacar el exceso. Ahora lo escondemos.
        int x = dim.x - camera.x + SCREEN_X_OFFSET;
        int y = dim.y - camera.y + SCREEN_Y_OFFSET;
        int max_x = x + dim.w;

        if (max_x > SCREEN_WIDTH) {
            int excess = max_x - SCREEN_WIDTH;
            SDL_Rect clip = {0, 0, dim.w - excess, dim.h};
            g_texture->render(x, y, &clip, &scale);
        } else {
            g_texture->render(x, y, NULL, &scale);
        }
        */
    }
}

Tile::~Tile() {}

//-----------------------------------------------------------------------------
