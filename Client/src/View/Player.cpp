#include "../../includes/View/Player.h"

//-----------------------------------------------------------------------------
// Métodos privados

bool Player::_movementNeeded() const {
    return (x != (data.x_tile * tile_w)) || (y != (data.y_tile * tile_h));
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
    x += this->x;
    y += this->y;

    // paso 3
    SDL_Rect render_quad, render_clip;
    SDL_Texture* texture;
    render_quad = {x, y, (int)(sprite.clip_w * scale_factor),
                   (int)(sprite.clip_h * scale_factor)};
    render_clip = {0, 0, sprite.clip_w, sprite.clip_h};
    texture = sprite.texture.getTexture();
    g_renderer->renderIfVisible(texture, &render_quad, &render_clip);
}

void Player::_updateMovement() {
    if (_movementNeeded()) {
        x = data.x_tile * tile_w;
        y = data.y_tile * tile_h;
    }
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
      y_vel(0) {}

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
    state = READY;

    _updateMovement();
}

void Player::act() {
    if (!state) {
        throw Exception("Player has not been initialized (act requested).");
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
    return SDL_Rect(
        {x, y, (int)(body_w * scale_factor), (int)(head_h * scale_factor)});
}

Player::~Player() {}

//-----------------------------------------------------------------------------
