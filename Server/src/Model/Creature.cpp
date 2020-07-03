#include <math.h>

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
                   ItemsContainer& items,
                   std::unordered_map<InstanceId, Character>& characters)
    : id(data.id),
      name(data.name),
      health_max(data.base_health),
      health(health),
      min_damage(data.min_damage),
      max_damage(data.max_damage),
      position(init_map, init_x_coord, init_y_coord, map_container),
      visible_range(data.visible_range),
      movement_speed(data.movement_speed),
      items(items),
      map(map_container[init_map]),
      characters(characters),
      is_moving(false),
      moving_cooldown(0),
      attribute_update_time_elapsed(0),
      attack_cooldown(data.attack_cooldown),
      actual_attack_cooldown(0),
      attacking_id(0),
      level(15),
      broadcast(false) {}

Creature::~Creature() {}

InstanceId Creature::_getNearestCharacter() {
    InstanceId nearest_id = 0;
    unsigned int nearest_range = visible_range + 1;
    const unsigned int x_tile = position.getX();
    const unsigned int y_tile = position.getY();
    for (int y = -int(visible_range); y <= int(visible_range); y++) {
        int abs_y = std::abs(y);
        for (int x = -int(visible_range) + abs_y;
             x <= int(visible_range) - abs_y; x++) {
            InstanceId id = 0;
            try {
                id = map.getTile(x_tile + x, y_tile + y).occupant_id;
            } catch (Exception& e) {
                continue;
            }
            if (characters.count(id) > 0) {
                if (x + y < int(nearest_range) &&
                    characters.at(id).getHealth() > 0) {
                    nearest_range = x + y;
                    nearest_id = id;
                }
            }
        }
    }

    return nearest_id;
}

bool Creature::_attackNearstCharacter(const Position& position_character) {
    if (position.getX() == position_character.getX()) {
        if (position_character.getY() - position.getY() == 1) {
            this->position.changeOrientation(DOWN_ORIENTATION);
            return true;
        }
        if (position.getY() - position_character.getY() == 1) {
            this->position.changeOrientation(UP_ORIENTATION);
            return true;
        }
    }
    if (position.getY() == position_character.getY()) {
        if (position_character.getX() - position.getX() == 1) {
            this->position.changeOrientation(RIGHT_ORIENTATION);
            return true;
        }
        if (position.getX() - position_character.getX() == 1) {
            this->position.changeOrientation(LEFT_ORIENTATION);
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
void Creature::_updateDamage(const unsigned int it, const InstanceId id) {
    actual_attack_cooldown -= it * RATE;
    attacking_id = 0;

    while (actual_attack_cooldown <= 0) {
        this->broadcast = true;
        RandomNumberGenerator random_number_generator;
        int damage = level * (int)random_number_generator(
                                 (int)this->min_damage, (int)this->max_damage);
        characters.at(id).receiveAttack(damage, true);
        actual_attack_cooldown += attack_cooldown;
        attacking_id = id;
    }
}

void Creature::_updateMovement(const unsigned int it) {
    this->moving_cooldown -= it * RATE;

    while (this->moving_cooldown <= 0) {
        this->broadcast = true;

        this->position.move();

        this->moving_cooldown += 1000 / movement_speed;
    }
}

void Creature::act(const unsigned int it) {
    InstanceId id = _getNearestCharacter();
    if (!id) {
        stopMoving();
    } else {
        if (_attackNearstCharacter(characters.at(id).getPosition())) {
            _updateDamage(it, id);
            stopMoving();
        } else {
            _determinateDirectionAndMove(characters.at(id).getPosition());
            attacking_id = 0;
        }
    }

    if (is_moving) {
        _updateMovement(it);
    }
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

const bool Creature::recoverHealth(unsigned int& points) {
    throw CantRecoverCreaturesHealthException();
}

const Position& Creature::getPosition() const {
    return this->position;
}

void Creature::dropAllItems(std::vector<DroppingSlot>& dropped_items) {
    RandomNumberGenerator gen;
    float prob = gen((float)0, (float)1);

    float acum = DROP_NOTHING_PROB;

    if (prob < acum) {
        fprintf(stderr, "DEBUG: Creature::drop: NOTHING\n");
        return;
    }

    acum += DROP_GOLD_PROB;

    if (prob < acum) {
        dropped_items.emplace_back(
            GOLD_BAG_ID,
            Formulas::calculateGoldDroppingAmount(this->health_max));
        fprintf(stderr, "DEBUG: Creature::drop: ORO. Item id: %i\n",
                GOLD_BAG_ID);
        return;
    }

    acum += DROP_POTION_PROB;

    if (prob < acum) {
        const std::vector<Id>& potions = this->items.getPotionsId();
        size_t potion_idx = gen((int)0, (int)potions.size() - 1);
        dropped_items.emplace_back(potions[potion_idx], 1);
        fprintf(stderr, "DEBUG: Creature::drop: POTION. Item id: %i\n",
                potions[potion_idx]);
        return;
    }

    int choice = gen((int)0, (int)2);

    if (choice < 1) {
        const std::vector<Id>& weapons = this->items.getWeaponsId();
        size_t weapon_idx = gen((int)0, (int)weapons.size() - 1);
        dropped_items.emplace_back(weapons[weapon_idx], 1);
        fprintf(stderr, "DEBUG: Creature::drop: WEAPON. Item id: %i\n",
                weapons[weapon_idx]);
        return;
    }

    if (choice < 2) {
        const std::vector<Id>& wands = this->items.getWandsId();
        size_t wand_idx = gen((int)0, (int)wands.size() - 1);
        dropped_items.emplace_back(wands[wand_idx], 1);
        fprintf(stderr, "DEBUG: Creature::drop: WAND. Item id: %i\n",
                wands[wand_idx]);
        return;
    }

    const std::vector<Id>& defences = this->items.getDefencesId();
    size_t defence_idx = gen((int)0, (int)defences.size() - 1);
    dropped_items.emplace_back(defences[defence_idx], 1);
    fprintf(stderr, "DEBUG: Creature::drop: DEFENCE. Item id: %i\n",
            defences[defence_idx]);
    return;
}

const unsigned int Creature::getMaxHealth() const {
    return health_max;
}

const unsigned int Creature::getHealth() const {
    return health;
}

const unsigned int Creature::getLevel() const {
    return this->level;
}

const Id Creature::getMapId() const {
    return this->position.getMapId();
}

const InstanceId Creature::getAttackingCharacterId() const {
    return this->attacking_id;
}

const bool Creature::isCreature() const {
    return true;
}

void Creature::fillBroadcastData(CreatureData& data) const {
    this->position.fillBroadcastData(data.basic_data);
    data.basic_data.movement_speed = this->movement_speed;
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
