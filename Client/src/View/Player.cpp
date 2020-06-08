#include "../../includes/View/Player.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Player::_moveUp() {
    if (predictor.canMoveTo(x_tile, y_tile - 1)) {
        y_tile--;
        state = MOVING;
        last_moved = SDL_GetTicks();
        next_x = box.x;
        next_y = _yValueToReach();
        y_vel = -PLAYER_MOVEMENT_SPEED;
        current_frame = 0;
        max_frames_current_animation = PLAYER_UP_SPRITES;
        row_current_animation = PLAYER_UP_ROW;
    }
}

void Player::_moveDown() {
    if (predictor.canMoveTo(x_tile, y_tile + 1)) {
        y_tile++;
        state = MOVING;
        last_moved = SDL_GetTicks();
        next_x = box.x;
        next_y = _yValueToReach();
        y_vel = PLAYER_MOVEMENT_SPEED;
        current_frame = 0;
        max_frames_current_animation = PLAYER_DOWN_SPRITES;
        row_current_animation = PLAYER_DOWN_ROW;
    }
}

void Player::_moveLeft() {
    if (predictor.canMoveTo(x_tile - 1, y_tile)) {
        x_tile--;
        state = MOVING;
        last_moved = SDL_GetTicks();
        next_x = _xValueToReach();
        next_y = box.y;
        x_vel = -PLAYER_MOVEMENT_SPEED;
        current_frame = 0;
        max_frames_current_animation = PLAYER_LEFT_SPRITES;
        row_current_animation = PLAYER_LEFT_ROW;
    }
}

void Player::_moveRight() {
    if (predictor.canMoveTo(x_tile + 1, y_tile)) {
        x_tile++;
        state = MOVING;
        last_moved = SDL_GetTicks();
        next_x = _xValueToReach();
        next_y = box.y;
        x_vel = PLAYER_MOVEMENT_SPEED;
        current_frame = 0;
        max_frames_current_animation = PLAYER_RIGHT_SPRITES;
        row_current_animation = PLAYER_RIGHT_ROW;
    }
}

bool Player::_isMovementCompleted() {
    if ((x_vel > 0) && (box.x >= next_x))
        return true;

    if ((x_vel < 0) && (box.x <= next_x))
        return true;

    if ((y_vel > 0) && (box.y >= next_y))
        return true;

    if ((y_vel < 0) && (box.y <= next_y))
        return true;

    return false;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Player::Player(const Renderer* renderer, const Predictor& predictor, int x_tile,
               int y_tile)
    : Unit(renderer, x_tile, y_tile),
      predictor(predictor),
      state(STANDING),
      frame_clip({0, 0, PLAYER_SPRITE_W, PLAYER_SPRITE_H}),
      current_frame(0),
      max_frames_current_animation(0),
      row_current_animation(0) {
    box.w = PLAYER_SPRITE_W;
    box.h = PLAYER_SPRITE_H;
    _centerBoxOnTile();
}

void Player::loadMedia() {
    sprite_sheet.loadFromFile(
        g_renderer, SPRITES_PATH + std::string("4032.png"), PLAYER_SPRITE_BG_R,
        PLAYER_SPRITE_BG_G, PLAYER_SPRITE_BG_B);
}

void Player::handleEvent(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_w:
                if (state == STANDING) {
                    _moveUp();
                }
                break;

            case SDLK_s:
                if (state == STANDING) {
                    _moveDown();
                }
                break;

            case SDLK_a:
                if (state == STANDING) {
                    _moveLeft();
                }
                break;

            case SDLK_d:
                if (state == STANDING) {
                    _moveRight();
                }
                break;
        }
    }
}

void Player::updatePosition(int corrected_x_tile, int corrected_y_tile) {
    // corregir posición de forma abrupta? ver... CORRECCIÓN DE PREDICCIONES
}

void Player::move() {
    if (state == MOVING) {
        Uint32 time = SDL_GetTicks();
        float time_step = (float)(time - last_moved); /* en ms */
        last_moved = time;
        box.x += x_vel * (time_step / 1000);
        box.y += y_vel * (time_step / 1000);

        current_frame++;
        if ((current_frame / PLAYER_FRAMES_BEFORE_CHANGE) ==
            max_frames_current_animation) {
            current_frame = 0;
        }

        if (_isMovementCompleted()) {
            box.x = next_x;
            box.y = next_y;
            x_vel = 0;
            y_vel = 0;
            current_frame = 0;
            state = STANDING;
        }

        frame_clip.x =
            (current_frame / PLAYER_FRAMES_BEFORE_CHANGE) * frame_clip.w;
        frame_clip.y = row_current_animation * frame_clip.h;

        fprintf(stderr, "Moviendo. CLIP = {x = %i, y = %i, w = %i, h = %i}\n",
                frame_clip.x, frame_clip.y, frame_clip.w, frame_clip.h);
    }
}

void Player::render() const {
    SDL_Rect render_quad = {box.x, box.y, sprite_sheet.getWidth(),
                            sprite_sheet.getHeight()};
    g_renderer->renderIfVisible(sprite_sheet.getTexture(), &render_quad,
                                &frame_clip);
}

SDL_Rect Player::getBox() const {
    return box;
}

SDL_Rect Player::getPos() const {
    return SDL_Rect({x_tile, y_tile, box.w, box.h});
}

Player::~Player() {}

//-----------------------------------------------------------------------------
