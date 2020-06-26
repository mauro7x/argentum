#include "../../includes/View/Character.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Character::_copyData(const CharacterData& init_data) {
    // Data básica
    data = init_data.basic_data;

    // Nombre de usuario
    nickname = init_data.nickname;

    // Cuerpo y cabeza
    head_id = init_data.head_id;
    body_id = init_data.body_id;

    // Inventario
    equipment = init_data.equipment;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Character::Character(Renderer* renderer, UnitSpriteContainer* sprites,
                     const TTF_Font* g_nickname_font,
                     const TTF_Font* g_level_font)
    : Unit(renderer, sprites),
      g_nickname_font(g_nickname_font),
      g_level_font(g_level_font) {}

void Character::init(const CharacterData& init_data) {
    if (state) {
        throw Exception("Character has already been initialized.");
    }

    /* Copiamos la data inicial */
    _copyData(init_data);

    /* Seteamos nuestra posición en pixeles para el renderizado */
    x = TILE_WIDTH * data.x_tile;
    y = TILE_HEIGHT * data.y_tile;

    /* Completamos la inicialización */
    state = READY;
}

void Character::update(const CharacterData& updated_data) {
    if (!state) {
        throw Exception(
            "Character has not been initialized (update requested).");
    }

    /* Actualizamos la data */
    _copyData(updated_data);

    /* Iniciamos el movimiento si nuestra posición en pixeles no coincide*/
    _setMovementSpeed();
}

void Character::render() const {
    if (!state) {
        throw Exception(
            "Character has not been initialized (render requested).");
    }

    // Cuerpo
    if (body_id) {
        _render(g_sprites->get(body_id));
    }

    // Armadura
    if (equipment[ARMOUR]) {
        _render(g_sprites->get(equipment[ARMOUR]));
    }

    // Escudo
    if (equipment[SHIELD]) {
        _render(g_sprites->get(equipment[SHIELD]));
    }

    // Espada
    if (equipment[WEAPON]) {
        _render(g_sprites->get(equipment[WEAPON]));
    }

    // Cabeza
    if (head_id) {
        _render(g_sprites->get(head_id));
    }

    // Casco
    if (equipment[HELMET]) {
        _render(g_sprites->get(equipment[HELMET]));
    }
}

Character::~Character() {}

//-----------------------------------------------------------------------------
