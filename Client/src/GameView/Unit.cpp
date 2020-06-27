#include "../../includes/GameView/Unit.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Unit::_setMovementSpeed() {
    int next_x = data.x_tile * TILE_WIDTH;
    int next_y = data.y_tile * TILE_HEIGHT;

    /* Velocidad en X */
    if (next_x > x) {
        x_vel = ((float)(next_x - x) / UNIT_TIME_TO_MOVE);
    } else if (next_x < x) {
        x_vel = -((float)(x - next_x) / UNIT_TIME_TO_MOVE);
    } else {
        x_vel = 0;
    }

    /* Velocidad en Y */
    if (next_y > y) {
        y_vel = ((float)(next_y - y) / UNIT_TIME_TO_MOVE);
    } else if (next_y < y) {
        y_vel = -((float)(y - next_y) / UNIT_TIME_TO_MOVE);
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
    int next_x = data.x_tile * TILE_WIDTH;
    int next_y = data.y_tile * TILE_HEIGHT;

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
        current_animation_frame = 0;
    }
}

int Unit::_calculateSpriteX(const UnitSprite& sprite) const {
    switch (data.orientation) {
        case UP_ORIENTATION:
            return (sprite.up_col +
                    ((current_animation_frame / sprite.change_every_n_frames) %
                     sprite.up_clips)) *
                   sprite.clip_w;
        case DOWN_ORIENTATION:
            return (sprite.down_col +
                    ((current_animation_frame / sprite.change_every_n_frames) %
                     sprite.down_clips)) *
                   sprite.clip_w;
        case LEFT_ORIENTATION:
            return (sprite.left_col +
                    ((current_animation_frame / sprite.change_every_n_frames) %
                     sprite.left_clips)) *
                   sprite.clip_w;
        case RIGHT_ORIENTATION:
            return (sprite.right_col +
                    ((current_animation_frame / sprite.change_every_n_frames) %
                     sprite.right_clips)) *
                   sprite.clip_w;
        default:
            throw Exception("Unit::_calculateSpriteX: invalid orientation.");
    }
}

int Unit::_calculateSpriteY(const UnitSprite& sprite) const {
    switch (data.orientation) {
        case UP_ORIENTATION:
            return sprite.up_row * sprite.clip_h;
        case DOWN_ORIENTATION:
            return sprite.down_row * sprite.clip_h;
        case LEFT_ORIENTATION:
            return sprite.left_row * sprite.clip_h;
        case RIGHT_ORIENTATION:
            return sprite.right_row * sprite.clip_h;
        default:
            throw Exception("Unit::_canculateSpriteY: invalid orientation.");
    }
}

void Unit::_render(const UnitSprite& sprite) const {
    // Paso 1: centramos el sprite en el tile
    int x = (TILE_WIDTH - (sprite.clip_w)) / 2;
    int y = (TILE_HEIGHT * (0.8)) - (sprite.clip_h);

    // Paso 2: agregamos el offset de la unidad en el mapa
    x += (int)this->x;
    y += (int)this->y;

    // Paso 3: armamos los SDL_Rect de renderizado
    SDL_Rect render_quad, render_clip;
    render_quad = {x, y, (int)(sprite.clip_w), (int)(sprite.clip_h)};
    render_clip = {0, 0, sprite.clip_w, sprite.clip_h};
    render_clip.x = _calculateSpriteX(sprite);
    render_clip.y = _calculateSpriteY(sprite);

    // Paso 4: renderizamos si somos visibles por la camara
    g_camera.renderIfVisible(g_renderer, sprite.texture.getTexture(),
                             render_quad, &render_clip);
}

//-----------------------------------------------------------------------------
// API Pública

Unit::Unit(Renderer* renderer, const Camera& camera,
           UnitSpriteContainer* sprites)
    : g_renderer(renderer), g_camera(camera), g_sprites(sprites) {}

void Unit::act(const int it) {
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
        current_animation_frame += it;

        /* Chequeamos si terminó */
        _movementFinished();
    }
}

SDL_Point Unit::getPos() const {
    if (!state) {
        throw Exception("Unit has not been initialized (pos requested).");
    }

    return SDL_Point({data.x_tile, data.y_tile});
}

Unit::~Unit() {}

//-----------------------------------------------------------------------------