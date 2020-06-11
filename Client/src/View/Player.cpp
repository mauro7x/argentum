#include "../../includes/View/Player.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Player::_setMovementSpeed() {
    int next_x = data.x_tile * tile_w;
    int next_y = data.y_tile * tile_h;

    /* Velocidad en X */
    if (next_x > x) {
        x_vel = ((float)(next_x - x) / TIME_TO_MOVE);
    } else if (next_x < x) {
        x_vel = -((float)(x - next_x) / TIME_TO_MOVE);
    } else {
        x_vel = 0;
    }

    /* Velocidad en Y */
    if (next_y > y) {
        y_vel = ((float)(next_y - y) / TIME_TO_MOVE);
    } else if (next_y < y) {
        y_vel = -((float)(y - next_y) / TIME_TO_MOVE);
    } else {
        y_vel = 0;
    }

    fprintf(stderr, "x_vel = %f, y_vel = %f\n", x_vel, y_vel);

    /* Si ya nos estabamos moviendo NO es necesario actualizar el last_moved */
    if ((state != MOVING) && ((x_vel != 0) || (y_vel != 0))) {
        /* Seteamos el estado y el last_moved */
        state = MOVING;
        last_moved = SDL_GetTicks();
    }
}

void Player::_checkIfMovementHasFinished() {
    int next_x = data.x_tile * tile_w;
    int next_y = data.y_tile * tile_h;

    if (((x_vel > 0) && (x > next_x)) || ((x_vel < 0) && (x < next_x))) {
        x = next_x;
        x_vel = 0;
    }

    if (((y_vel > 0) && (y > next_y)) || ((y_vel < 0) && (y < next_y))) {
        y = next_y;
        y_vel = 0;
    }

    if ((x == next_x) && (y == next_y)) {
        state = READY;
    }
}

void Player::_setScaleFactor() {
    int body_w = g_sprites[data.body_id].clip_w;
    if (body_w > tile_w) {
        scale_factor = tile_w / body_w;
    } else {
        scale_factor = 1.0;
    }
}

void Player::_render(const Sprite& sprite) const {
    /** Pasos para renderizar cada componente:
     *
     * 1. Obtener (x, y) centrado en el tile aplicando scale_factor.
     * 2. Agregar el offset (x, y) del player.
     * 3. Renderizar aplicando scale_factor.
     */

    int x, y;

    // paso 1
    x = (tile_w - (sprite.clip_w * scale_factor)) / 2;
    y = (tile_h * (0.8)) - (sprite.clip_h * scale_factor);

    // paso 2
    x += (int)this->x;
    y += (int)this->y;

    // paso 3
    SDL_Rect render_quad, render_clip;
    SDL_Texture* texture;
    render_quad = {x, y, (int)(sprite.clip_w * scale_factor),
                   (int)(sprite.clip_h * scale_factor)};
    render_clip = {0, 0, sprite.clip_w, sprite.clip_h};
    texture = sprite.texture.getTexture();
    g_renderer->renderIfVisible(texture, &render_quad, &render_clip);
}

// OLD API --------------------------------------------------------------------

/*
void Player::_centerOnTile() {
    // x = (data.x_tile * tile_w) + ((tile_w - w) / 2);
    // y = (data.y_tile * tile_h) + (tile_h * (0.8)) - h;
}

int Player::_xValueToReach() const {
    return (data.x_tile * tile_w) + ((tile_w - box.w) / 2);
}

int Player::_yValueToReach() const {
    return (data.y_tile * tile_h) + (tile_h * (0.8)) - box.h;
}
*/

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Player::Player(const Renderer* renderer, const UnitSpriteContainer& sprites)
    : g_renderer(renderer),
      g_sprites(sprites),
      state(NOT_INIT),
      x(0),
      y(0),
      x_vel(0),
      y_vel(0),
      last_moved(0) {}

void Player::init(const PlayerData& init_data) {
    if (state) {
        throw Exception("Player has already been initialized.");
    }

    data = init_data;
    json map_data = JSON::loadJsonFile(MAPS_FILEPATH);
    tile_w = map_data["tilewidth"];
    tile_h = map_data["tileheight"];
    x = tile_w * data.x_tile;
    y = tile_h * data.y_tile;
    _setScaleFactor();
    state = READY;
}

void Player::update(const PlayerData& updated_data) {
    if (!state) {
        throw Exception("Player has not been initialized (update requested).");
    }

    data = updated_data;
    _setScaleFactor();
    _setMovementSpeed();
}

void Player::act() {
    if (!state) {
        throw Exception("Player has not been initialized (act requested).");
    }

    if (state == MOVING) {
        Uint32 time, time_step;
        time = SDL_GetTicks();
        time_step = time - last_moved;

        fprintf(stderr,
                "ANTES: x = %i, y = %i, x_vel = %f, y_vel = %f, mov_x "
                "= %f, mov_y = %f\n",
                (int)x, (int)y, x_vel, y_vel, (x_vel * time_step),
                (y_vel * time_step));
        x += x_vel * time_step;
        y += y_vel * time_step;
        fprintf(stderr, "x= %i, y= %i\n", (int)x, (int)y);
        last_moved = time;
        _checkIfMovementHasFinished();
    }
}

void Player::render() const {
    if (!state) {
        throw Exception("Player has not been initialized (render requested).");
    }

    // Cuerpo
    if (data.body_id) {
        _render(g_sprites[data.body_id]);
    }

    // Armadura
    if (data.armour_id) {
        _render(g_sprites[data.armour_id]);
    }

    // Escudo
    if (data.shield_id) {
        _render(g_sprites[data.shield_id]);
    }

    // Espada
    if (data.weapon_id) {
        _render(g_sprites[data.weapon_id]);
    }

    // Cabeza
    if (data.head_id) {
        _render(g_sprites[data.head_id]);
    }

    // Casco
    if (data.helmet_id) {
        _render(g_sprites[data.helmet_id]);
    }
}

SDL_Rect Player::getPos() const {
    if (!state) {
        throw Exception("Player has not been initialized (pos requested).");
    }

    return SDL_Rect({data.x_tile, data.y_tile, 0, 0});
}

SDL_Rect Player::getBox() const {
    if (!state) {
        throw Exception("Player has not been initialized (box requested).");
    }

    int body_w = g_sprites[data.body_id].clip_w;
    int head_h = g_sprites[data.head_id].clip_h;
    return SDL_Rect({(int)x, (int)y, (int)(body_w * scale_factor),
                     (int)(head_h * scale_factor)});
}

Player::~Player() {}

//-----------------------------------------------------------------------------
