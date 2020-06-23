#include "../../includes/View/Creature.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Creature::_copyData(const CreatureData& init_data) {
    // Data básica
    data = init_data.basic_data;

    // Ids gráficos
    creature_id = init_data.creature_id;

    // Vida
    health = init_data.health;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Creature::Creature(Renderer* renderer, UnitSpriteContainer* sprites)
    : Unit(renderer, sprites) {}

Creature::Creature(Creature&& other) : Unit(std::move(other)) {
    creature_id = other.creature_id;
    health = other.health;
}

Creature& Creature::operator=(Creature&& other) {
    Unit::operator=(std::move(other));
    creature_id = other.creature_id;
    health = other.health;

    return *this;
}

void Creature::init(const CreatureData& init_data) {
    if (state) {
        throw Exception("Creature has already been initialized.");
    }

    /* Copiamos la data inicial */
    _copyData(init_data);

    /* Seteamos nuestra posición en pixeles para el renderizado */
    x = TILE_WIDTH * data.x_tile;
    y = TILE_HEIGHT * data.y_tile;

    /* Completamos la inicialización */
    state = READY;
}

void Creature::update(const CreatureData& updated_data) {
    if (!state) {
        throw Exception(
            "Creature has not been initialized (update requested).");
    }

    /* Actualizamos la data */
    _copyData(updated_data);

    /* Iniciamos el movimiento si nuestra posición en pixeles no coincide*/
    _setMovementSpeed();
}

void Creature::render() const {
    if (!state) {
        throw Exception(
            "Creature has not been initialized (render requested).");
    }

    // Cuerpo
    _render(g_sprites->get(creature_id));
}

Creature::~Creature() {}

//-----------------------------------------------------------------------------
