#include "../../includes/View/Character.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Character::_copyData(const CharacterData& init_data) {
    // Data básica
    data = init_data.basic_data;

    // Ids gráficos
    head_id = init_data.head_id;
    body_id = init_data.body_id;
    helmet_id = init_data.helmet_id;
    armour_id = init_data.armour_id;
    shield_id = init_data.shield_id;
    weapon_id = init_data.weapon_id;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Character::Character(Renderer* renderer, UnitSpriteContainer* sprites,
                     const int tile_w, const int tile_h,
                     const float tile_movement_time)
    : Unit(renderer, sprites, tile_w, tile_h, tile_movement_time),
      head_id(0),
      body_id(0),
      helmet_id(0),
      armour_id(0),
      shield_id(0),
      weapon_id(0) {}

Character::Character(Character&& other) : Unit(std::move(other)) {
    head_id = other.head_id;
    body_id = other.body_id;
    helmet_id = other.helmet_id;
    armour_id = other.armour_id;
    shield_id = other.shield_id;
    weapon_id = other.weapon_id;
}

Character& Character::operator=(Character&& other) {
    Unit::operator=(std::move(other));
    head_id = other.head_id;
    body_id = other.body_id;
    helmet_id = other.helmet_id;
    armour_id = other.armour_id;
    shield_id = other.shield_id;
    weapon_id = other.weapon_id;

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
    if (armour_id) {
        _render(g_sprites->get(armour_id));
    }

    // Escudo
    if (shield_id) {
        _render(g_sprites->get(shield_id));
    }

    // Espada
    if (weapon_id) {
        _render(g_sprites->get(weapon_id));
    }

    // Cabeza
    if (head_id) {
        _render(g_sprites->get(head_id));
    }

    // Casco
    if (helmet_id) {
        _render(g_sprites->get(helmet_id));
    }
}

Character::~Character() {}

//-----------------------------------------------------------------------------
