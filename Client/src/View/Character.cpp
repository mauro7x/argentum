#include "../../includes/View/Character.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Character::_copyData(const CharacterData& data) {
    x_tile = data.x_tile;
    y_tile = data.y_tile;
    head_id = data.head_id;
    body_id = data.body_id;
    helmet_id = data.helmet_id;
    armour_id = data.armour_id;
    shield_id = data.shield_id;
    weapon_id = data.shield_id;
}

void Character::_setScaleFactor() {
    int body_w = g_sprites->get(body_id).clip_w;
    if (body_w > tile_w) {
        scale_factor = tile_w / body_w;
    } else {
        scale_factor = 1.0;
    }
}

void Character::_render(const Sprite& sprite) const {
    /** Pasos para renderizar cada componente:
     *
     * 1. Obtener (x, y) centrado en el tile aplicando scale_factor.
     * 2. Agregar el offset (x, y) del character.
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
    render_quad = {x, y, (sprite.clip_w * scale_factor),
                   (sprite.clip_h * scale_factor)};
    render_clip = {0, 0, sprite.clip_w, sprite.clip_h};
    texture = sprite.texture.getTexture();
    g_renderer->renderIfVisible(texture, &render_quad, &render_clip);
}

void Character::_startMovementIfNeeded() {
    /* si el x no concuerda con x_tile * tile_w, o lo mismo con y, mover */
}

// OLD API --------------------------------------------------------------------

/*
void Character::_centerOnTile() {
    // x = (x_tile * tile_w) + ((tile_w - w) / 2);
    // y = (y_tile * tile_h) + (tile_h * (0.8)) - h;
}

int Character::_xValueToReach() const {
    return (x_tile * tile_w) + ((tile_w - box.w) / 2);
}

int Character::_yValueToReach() const {
    return (y_tile * tile_h) + (tile_h * (0.8)) - box.h;
}
*/

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Character::Character(Renderer* renderer, UnitSpriteContainer* sprites,
                     const CharacterData& data, const int tile_w,
                     const int tile_h)
    : g_renderer(renderer), g_sprites(sprites), tile_w(tile_w), tile_h(tile_h) {
    _copyData(data);
    x = tile_w * x_tile;
    y = tile_h * y_tile;
    _setScaleFactor();
}

Character::Character(Character&& other) {
    /* Nos apropiamos de los punteros */
    g_renderer = other.g_renderer;
    g_sprites = other.g_sprites;
    other.g_renderer = NULL;
    other.g_sprites = NULL;

    /* Copiamos el resto de atributos */
    tile_w = other.tile_w;
    tile_h = other.tile_h;
    x_tile = other.x_tile;
    y_tile = other.y_tile;
    head_id = other.head_id;
    body_id = other.body_id;
    helmet_id = other.helmet_id;
    armour_id = other.armour_id;
    shield_id = other.shield_id;
    weapon_id = other.weapon_id;
}

Character& Character::operator=(Character&& other) {
    /* Nos apropiamos de los punteros */
    g_renderer = other.g_renderer;
    g_sprites = other.g_sprites;
    other.g_renderer = NULL;
    other.g_sprites = NULL;

    /* Copiamos el resto de atributos */
    tile_w = other.tile_w;
    tile_h = other.tile_h;
    x_tile = other.x_tile;
    y_tile = other.y_tile;
    head_id = other.head_id;
    body_id = other.body_id;
    helmet_id = other.helmet_id;
    armour_id = other.armour_id;
    shield_id = other.shield_id;
    weapon_id = other.weapon_id;

    return *this;
}

void Character::update(const CharacterData& updated_data) {
    _copyData(updated_data);
    _setScaleFactor();
    _startMovementIfNeeded();
}

void Character::act() {}

void Character::render() const {
    // Cuerpo
    _render(g_sprites->get(body_id));
}

Character::~Character() {}

//-----------------------------------------------------------------------------
