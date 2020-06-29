#include <algorithm>
//-----------------------------------------------------------------------------
#include "../../includes/Model/Creature.h"
#include "../../includes/Model/Formulas.h"
//-----------------------------------------------------------------------------
#include "../../../Common/includes/RandomNumberGenerator.h"
//-----------------------------------------------------------------------------
#define DEFAULT_MOVING_ORIENTATION DOWN_ORIENTATION
//-----------------------------------------------------------------------------
Creature::Creature(const CreatureCfg& data, MapContainer& map_container,
                   const Id init_map, const int init_x_coord,
                   const int init_y_coord, const uint32_t health,
                   const uint32_t damage, ItemsContainer& items)
    : id(data.id),
      name(data.name),
      health_max(data.base_health),
      health(health),
      damage(damage),
      position(init_map, init_x_coord, init_y_coord, map_container),
      items(items),
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

const bool Creature::receiveAttack(int& damage, const bool eludible) {
    health = std::max(0, (int)(health - damage));
    this->broadcast = true;
    return true;
}

void Creature::beAttacked() {}

const bool Creature::isNewbie() const {
    return false;
}

void Creature::recoverHealth(const unsigned int points) {
    throw CantRecoverCreaturesHealthException();
}

const Position& Creature::getPosition() const {
    return this->position;
}

void Creature::dropAllItems(std::vector<DroppingSlot>& dropped_items) {
    RandomNumberGenerator gen;
    float prob = gen(0, 1);

    float acum = DROP_NOTHING_PROB;

    if (prob < acum) {
        return;
    }

    acum += DROP_GOLD_PROB;


    if (prob < acum) {
        dropped_items.emplace_back(
            GOLD_BAG_ID,
            Formulas::calculateGoldDroppingAmount(this->health_max));
        return;
    }

    acum += DROP_POTION_PROB;

    if (prob < acum) {
        const std::vector<Id>& potions = this->items.getPotionsId();
        dropped_items.emplace_back(potions[gen(0, potions.size() - 1)], 1);
        return;
    }

    float choice = gen(0, 3);
    
    if (choice < 1) {
        const std::vector<Id>& weapons = this->items.getWeaponsId();
        dropped_items.emplace_back(weapons[gen(0, weapons.size() - 1)], 1);
        return;
    }

    if (choice < 2) {
        const std::vector<Id>& wands = this->items.getWandsId();
        dropped_items.emplace_back(wands[gen(0, wands.size() - 1)], 1);
        return;
    }

    
    const std::vector<Id>& defences = this->items.getDefencesId();
    dropped_items.emplace_back(defences[gen(0, defences.size() - 1)], 1);
    return;
}

const unsigned int Creature::getMaxHealth() const {
    return health_max;
}

const unsigned int Creature::getHealth() const {
    return health;
}

const unsigned int Creature::getLevel() const {
    // IMPLEMENTAR NIVEL!!!!
    // IMPLEMENTAR NIVEL!!!!
    // IMPLEMENTAR NIVEL!!!!
    // IMPLEMENTAR NIVEL!!!!
    return 0;
}

const Id Creature::getMapId() const {
    return this->position.getMapId();
}

void Creature::fillBroadcastData(CreatureData& data) const {
    this->position.fillBroadcastData(data.basic_data);
    data.name = this->name;
    data.creature_id = this->id;
    data.health = this->health;
    data.max_health = this->health_max;
}

const bool Creature::mustBeBroadcasted() const {
    return this->broadcast;
}

void Creature::beBroadcasted() {
    this->broadcast = false;
}

const char* CantRecoverCreaturesHealthException::what() const noexcept {
    return "No se puede curar a una criatura.";
}
