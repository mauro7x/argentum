//-----------------------------------------------------------------------------
#include "../../includes/Model/Creature.h"

#include "../../includes/Model/Formulas.h"
//-----------------------------------------------------------------------------
#define DEFAULT_MOVING_ORIENTATION DOWN_ORIENTATION
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Creature::Creature(const CreatureCfg& data, MapContainer& map_container,
                   const Id init_map, const int init_x_coord,
                   const int init_y_coord, const unsigned int health,
                   const unsigned int damage)
    : id(data.id),
      name(data.name),
      health_max(data.base_health),
      health_actual(health),
      damage(damage),
      position(init_map, init_x_coord, init_y_coord, map_container),
      moving(false),
      moving_time_elapsed(0),
      attribute_update_time_elapsed(0) {}

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
}

unsigned int Creature::getMaxHealth() {
    return health_max;
}
