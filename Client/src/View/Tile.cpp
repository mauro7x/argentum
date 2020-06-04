#include "../../includes/View/Tile.h"

//-----------------------------------------------------------------------------
// Métodos privados

bool Tile::_checkCollision(const SDL_Rect& box) const {
    int left_me, left_box;
    int right_me, right_box;
    int top_me, top_box;
    int bottom_me, bottom_box;

    // Calculate the sides of rect A
    left_me = mBox.x;
    right_me = mBox.x + mBox.w;
    top_me = mBox.y;
    bottom_me = mBox.y + mBox.h;

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
    : gTexture(texture), gRenderer(renderer) {
    mBox.x = x;
    mBox.y = y;
    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;
}

Tile::Tile(Tile&& other) {
    this->gTexture = std::move(other.gTexture);
    this->gRenderer = std::move(other.gRenderer);
    this->mBox = std::move(other.mBox);
    other.gTexture = NULL;
    other.gRenderer = NULL;
}

Tile& Tile::operator=(Tile&& other) {
    this->gTexture = std::move(other.gTexture);
    this->gRenderer = std::move(other.gRenderer);
    this->mBox = std::move(other.mBox);
    other.gTexture = NULL;
    other.gRenderer = NULL;
    return *this;
}

void Tile::render(const SDL_Rect& camera) const {
    if (_checkCollision(camera)) {
        SDL_Rect scale = {0, 0, TILE_WIDTH, TILE_HEIGHT};

        int x = mBox.x - camera.x + SCREEN_X_OFFSET;
        int y = mBox.y - camera.y + SCREEN_Y_OFFSET;
        int max_x = x + mBox.w;

        if (max_x > SCREEN_WIDTH) {
            int excess = max_x - SCREEN_WIDTH;
            SDL_Rect clip = {0, 0, mBox.w - excess, mBox.h};
            gTexture->render(x, y, &clip, &scale);
        } else {
            gTexture->render(x, y, NULL, &scale);
        }
        /*
        gTexture->render(mBox.x - camera.x + SCREEN_X_OFFSET,
                             mBox.y - camera.y + SCREEN_Y_OFFSET, NULL, &scale);
        */
    }
}

Tile::~Tile() {}

//-----------------------------------------------------------------------------
