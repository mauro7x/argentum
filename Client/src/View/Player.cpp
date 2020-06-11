#include "../../includes/View/Player.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Player::_setMovementSpeed() {
    int next_x = data.x_tile * tile_w;
    int next_y = data.y_tile * tile_h;

    /* Velocidad en X */
    if (next_x > x) {
        x_vel = ((float)(next_x - x) / tile_movement_time);
    } else if (next_x < x) {
        x_vel = -((float)(x - next_x) / tile_movement_time);
    } else {
        x_vel = 0;
    }

    /* Velocidad en Y */
    if (next_y > y) {
        y_vel = ((float)(next_y - y) / tile_movement_time);
    } else if (next_y < y) {
        y_vel = -((float)(y - next_y) / tile_movement_time);
    } else {
        y_vel = 0;
    }

    /* Si ya nos estabamos moviendo NO es necesario actualizar el last_moved */
    if ((state != MOVING) && ((x_vel != 0) || (y_vel != 0))) {
        /* Seteamos el estado y el last_moved */
        state = MOVING;
        last_moved = SDL_GetTicks();
    }
}

void Player::_movementFinished() {
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
        last_tick = 0;
    }
}

int Player::_calculateSpriteX(const Sprite& sprite) const {
    switch (data.orientation) {
        case UP:
            return (sprite.up_col +
                    ((last_tick / sprite.change_every_n_frames) %
                     sprite.up_clips)) *
                   sprite.clip_w;
        case DOWN:
            return (sprite.down_col +
                    ((last_tick / sprite.change_every_n_frames) %
                     sprite.down_clips)) *
                   sprite.clip_w;
        case LEFT:
            return (sprite.left_col +
                    ((last_tick / sprite.change_every_n_frames) %
                     sprite.left_clips)) *
                   sprite.clip_w;
        case RIGHT:
            return (sprite.right_col +
                    ((last_tick / sprite.change_every_n_frames) %
                     sprite.right_clips)) *
                   sprite.clip_w;
    }

    throw Exception(
        "Orientation invalid (trying to calculate sprite x-position).");
}

int Player::_calculateSpriteY(const Sprite& sprite) const {
    switch (data.orientation) {
        case UP:
            return sprite.up_row * sprite.clip_h;
        case DOWN:
            return sprite.down_row * sprite.clip_h;
        case LEFT:
            return sprite.left_row * sprite.clip_h;
        case RIGHT:
            return sprite.right_row * sprite.clip_h;
    }

    throw Exception(
        "Orientation invalid (trying to calculate sprite y-position).");
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
    render_clip.x = _calculateSpriteX(sprite);
    render_clip.y = _calculateSpriteY(sprite);
    texture = sprite.texture.getTexture();
    g_renderer->renderIfVisible(texture, &render_quad, &render_clip);
}

//-----------------------------------------------------------------------------
// API Pública

Player::Player(const Renderer* renderer, const UnitSpriteContainer& sprites)
    : g_renderer(renderer),
      g_sprites(sprites),
      state(NOT_INIT),
      x(0),
      y(0),
      tile_movement_time(0),
      x_vel(0),
      y_vel(0),
      last_moved(0),
      last_tick(0) {}

void Player::init(const PlayerData& init_data) {
    if (state) {
        throw Exception("Player has already been initialized.");
    }

    data = init_data;

    /* Cargamos dimensiones del tile */
    json map_data = JSON::loadJsonFile(MAPS_FILEPATH);
    tile_w = map_data["tilewidth"];
    tile_h = map_data["tileheight"];

    /* Con ellas, seteamos nuestra posición en pixeles para el renderizado */
    x = tile_w * data.x_tile;
    y = tile_h * data.y_tile;

    /* Cargamos velocidad */
    json common_config = JSON::loadJsonFile(COMMON_CONFIG_FILEPATH);
    int speed = common_config["tiles_per_sec"]["character_speed"]; /* tiles/s */
    tile_movement_time = 1000 / speed;                             /* ms */

    /* Nos fijamos si somos demasiado grandes para el tile */
    _setScaleFactor();

    /* Completamos la inicialización */
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

void Player::act(const int tick) {
    if (!state) {
        throw Exception("Player has not been initialized (act requested).");
    }

    if (state == MOVING) {
        Uint32 time, time_step;
        time = SDL_GetTicks();
        time_step = time - last_moved;

        /* Movemos al personaje */
        x += x_vel * time_step;
        y += y_vel * time_step;
        last_moved = time;
        last_tick = tick;

        /* Chequeamos si terminó */
        _movementFinished();
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
