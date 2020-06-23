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
                     const int tile_w, const int tile_h)
    : Unit(renderer, sprites, tile_w, tile_h),
      head_id(0),
      body_id(0),
      equipment({0}) {}

Character::Character(Character&& other) : Unit(std::move(other)) {
    nickname = other.nickname;
    head_id = other.head_id;
    body_id = other.body_id;
    equipment = other.equipment;
}

Character& Character::operator=(Character&& other) {
    Unit::operator=(std::move(other));
    nickname = other.nickname;
    head_id = other.head_id;
    body_id = other.body_id;
    equipment = other.equipment;

    return *this;
}

void Character::init(const CharacterData& init_data) {
    if (state) {
        throw Exception("Character has already been initialized.");
    }

    /* Copiamos la data inicial */
    _copyData(init_data);

    /* Seteamos nuestra posición en pixeles para el renderizado */
    x = tile_w * data.x_tile;
    y = tile_h * data.y_tile;

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
