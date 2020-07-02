#include <algorithm>
#include <math.h>

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
                   const uint32_t damage, ItemsContainer& items,
                   std::unordered_map<InstanceId, Character>& characters)
    : id(data.id),
      name(data.name),
      health_max(data.base_health),
      health(health),
      damage(damage),
      position(init_map, init_x_coord, init_y_coord, map_container),
      visible_range(data.visible_range),
      movement_speed(data.movement_speed),
      items(items),
      characters(characters),
      is_moving(false),
      moving_cooldown(0),
      attribute_update_time_elapsed(0),
      attack_cooldown(0),
      broadcast(false) {}

Creature::~Creature() {}

InstanceId Creature::_getNearestCharacter() {
    InstanceId nearest_id = 0;
    unsigned int nearest_range = visible_range + 1;
    std::unordered_map<InstanceId, Character>::iterator it_characters =
        this->characters.begin();
    while (it_characters != this->characters.end()) {
        const unsigned int actual_range =
            position.getRange(it_characters->second.getPosition());
        const unsigned int health = it_characters->second.getHealth();
        if (actual_range <= visible_range) {
            if (actual_range < nearest_range && health > 0) {
                nearest_id = it_characters->first;
                nearest_range = actual_range;
            }
        }
        ++it_characters;
    }
    return nearest_id;
}

bool Creature::_attackNearstCharacter(const Position& position_character) {
    if (position.getX() == position_character.getX()) {
        if (position_character.getY() - position.getY() == 1) {
            this->position.changeOrientation(DOWN_ORIENTATION);
            // atacar
            return true;
        }
        if (position.getY() - position_character.getY() == 1) {
            this->position.changeOrientation(UP_ORIENTATION);
            // atacar
            return true;
        }
    }
    if (position.getY() == position_character.getY()) {
        if (position_character.getX() - position.getX() == 1) {
            this->position.changeOrientation(RIGHT_ORIENTATION);
            // atacar
            return true;
        }
        if (position.getY() - position_character.getY() == 1) {
            this->position.changeOrientation(LEFT_ORIENTATION);
            // atacar
            return true;
        }
    }
    return false;
}

void Creature::_determinateDirectionAndMove(
    const Position& position_character) {
    if (abs(position_character.getX() - position.getX()) >
        abs(position_character.getY() - position.getY())) {
        if (position_character.getX() > position.getX()) {
            startMovingRight();
        } else {
            startMovingLeft();
        }
    } else {
        if (position_character.getY() > position.getY()) {
            startMovingDown();
        } else {
            startMovingUp();
        }
    }
}

void Creature::_updateMovement(const unsigned int it) {
    this->moving_cooldown -= it * RATE;

    while (this->moving_cooldown <= 0) {
        this->broadcast = true;

        this->position.move();

        this->moving_cooldown += UNIT_TIME_TO_MOVE;
    }
}

void Creature::act(const unsigned int it) {
    InstanceId id = _getNearestCharacter();
    if (!id) {
        stopMoving();
    } else {
        if (_attackNearstCharacter(characters.at(id).getPosition())) {
            stopMoving();
        } else {
            _determinateDirectionAndMove(characters.at(id).getPosition());
        }
    }
    if (is_moving) {
        _updateMovement(it);
    } else {
        if (moving_cooldown > 0) {
            moving_cooldown =
                std::max(moving_cooldown - 1000/movement_speed, 0);
        }
    }

    if (attack_cooldown > 0)
        attack_cooldown = std::max(attack_cooldown - 1000/movement_speed, 0);
}

void Creature::startMovingUp() {
    this->position.changeOrientation(UP_ORIENTATION);
    this->is_moving = true;
}

void Creature::startMovingDown() {
    this->position.changeOrientation(DOWN_ORIENTATION);
    this->is_moving = true;
}

void Creature::startMovingRight() {
    this->position.changeOrientation(RIGHT_ORIENTATION);
    this->is_moving = true;
}

void Creature::startMovingLeft() {
    this->position.changeOrientation(LEFT_ORIENTATION);
    this->is_moving = true;
}

void Creature::stopMoving() {
    this->is_moving = false;
    this->moving_cooldown = 0;
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

const bool Creature::recoverHealth(const unsigned int points) {
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
