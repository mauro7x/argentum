//-----------------------------------------------------------------------------
#include "../../includes/Model/Creature.h"

#include "../../includes/Model/Formulas.h"
//-----------------------------------------------------------------------------
#define DEFAULT_MOVING_ORIENTATION DOWN_ORIENTATION
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Creature::Creature(const CreatureCfg& data, MapContainer& map_container,
                   const Id init_map, const int init_x_coord,
                   const int init_y_coord, const uint32_t health,
                   const uint32_t damage)
    : id(data.id),
      name(data.name),
      health_max(data.base_health),
      health_actual(health),
      damage(damage),
      position(init_map, init_x_coord, init_y_coord, map_container),
      moving(false),
      moving_time_elapsed(0),
      attribute_update_time_elapsed(0),
      broadcast(false) {}

Creature::~Creature() {}

void Creature::act(const unsigned int it) {
    // IMPLEMENTAR
}

void Creature::startMovingUp() {
    this->moving_orientation = UP_ORIENTATION;
    this->moving = true;
}

void Creature::startMovingDown() {
    this->moving_orientation = DOWN_ORIENTATION;
    this->moving = true;
}

void Creature::startMovingRight() {
    this->moving_orientation = RIGHT_ORIENTATION;
    this->moving = true;
}

void Creature::startMovingLeft() {
    this->moving_orientation = LEFT_ORIENTATION;
    this->moving = true;
}

void Creature::stopMoving() {
    this->moving = false;
    this->moving_time_elapsed = 0;
}

void Creature::receiveAttack(const unsigned int damage) {
    if (damage >= health_actual) {
        health_actual = 0;
    } else {
        health_actual -= damage;
    }

    this->broadcast = true;
}

unsigned int Creature::getMaxHealth() {
    return health_max;
}

void Creature::fillBroadcastData(CreatureData& data) const {
    this->position.fillBroadcastData(data.basic_data);
    data.name = this->name;
    data.creature_id = this->id;
    data.health = this->health_actual;
    data.max_health = this->health_max;
}

const bool Creature::mustBeBroadcasted() const {
    return this->broadcast;
}

void Creature::beBroadcasted() {
    this->broadcast = false;
}
