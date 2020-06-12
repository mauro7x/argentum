#include "../../includes/View/Unit.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Unit::_setMovementSpeed() {
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

void Unit::_movementFinished() {
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

int Unit::_calculateSpriteX(const Sprite& sprite) const {
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

int Unit::_calculateSpriteY(const Sprite& sprite) const {
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

void Unit::_setScaleFactor(const Sprite& sprite) {
    int body_w = sprite.clip_w;
    if (body_w > tile_w) {
        scale_factor = tile_w / body_w;
    } else {
        scale_factor = 1.0;
    }
}

void Unit::_render(const Sprite& sprite) const {
    // Paso 1: centramos el sprite en el tile
    int x = (tile_w - (sprite.clip_w * scale_factor)) / 2;
    int y = (tile_h * (0.8)) - (sprite.clip_h * scale_factor);

    // Paso 2: agregamos el offset de la unidad en el mapa
    x += (int)this->x;
    y += (int)this->y;

    // Paso 3: armamos los SDL_Rect de renderizado
    SDL_Rect render_quad, render_clip;
    SDL_Texture* texture;
    render_quad = {x, y, (int)(sprite.clip_w * scale_factor),
                   (int)(sprite.clip_h * scale_factor)};
    render_clip = {0, 0, sprite.clip_w, sprite.clip_h};
    render_clip.x = _calculateSpriteX(sprite);
    render_clip.y = _calculateSpriteY(sprite);

    // Paso 4: renderizamos
    texture = sprite.texture.getTexture();
    g_renderer->renderIfVisible(texture, &render_quad, &render_clip);
}

//-----------------------------------------------------------------------------
// API Pública

Unit::Unit(Renderer* renderer, UnitSpriteContainer* sprites, const int tile_w,
           const int tile_h, const int tile_movement_time)
    : g_renderer(renderer),
      g_sprites(sprites),
      tile_w(tile_w),
      tile_h(tile_h),
      data({0}),
      state(NOT_INIT),
      x(0),
      y(0),
      tile_movement_time(tile_movement_time),
      x_vel(0),
      y_vel(0),
      last_moved(0),
      last_tick(0) {}

Unit::Unit(Unit&& other) {
    g_renderer = other.g_renderer;
    other.g_renderer = NULL;
    g_sprites = other.g_sprites;
    g_sprites = NULL;
    tile_w = other.tile_w;
    tile_h = other.tile_h;
    data = other.data;
    state = other.state;
    x = other.x;
    y = other.y;
    tile_movement_time = other.tile_movement_time;
    scale_factor = other.scale_factor;
    x_vel = other.x_vel;
    y_vel = other.y_vel;
    last_moved = other.last_moved;
    last_tick = other.last_tick;
}

Unit& Unit::operator=(Unit&& other) {
    g_renderer = other.g_renderer;
    other.g_renderer = NULL;
    g_sprites = other.g_sprites;
    g_sprites = NULL;
    tile_w = other.tile_w;
    tile_h = other.tile_h;
    data = other.data;
    state = other.state;
    x = other.x;
    y = other.y;
    tile_movement_time = other.tile_movement_time;
    scale_factor = other.scale_factor;
    x_vel = other.x_vel;
    y_vel = other.y_vel;
    last_moved = other.last_moved;
    last_tick = other.last_tick;

    return *this;
}

void Unit::act(const int tick) {
    if (!state) {
        throw Exception("Unit has not been initialized (act requested).");
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

Unit::~Unit() {}

//-----------------------------------------------------------------------------
