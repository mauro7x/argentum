#include <math.h>

#include <algorithm>
//-----------------------------------------------------------------------------
#include "../../includes/Model/Creature.h"
#include "../../includes/Model/Formulas.h"
#include "../../includes/Model/Game.h"
//-----------------------------------------------------------------------------
#include "../../../Common/includes/RandomNumberGenerator.h"
//-----------------------------------------------------------------------------
#define DEFAULT_MOVING_ORIENTATION DOWN_ORIENTATION
//-----------------------------------------------------------------------------
Creature::Creature(const CreatureCfg& data, LogicMaps& logic_maps,
                   const Id init_map, const int init_x_coord,
                   const int init_y_coord, const uint32_t health,
                   ItemsContainer& items,
                   std::unordered_map<InstanceId, Character>& characters,
                   Game& game, Formulas& formulas, const int& rate,
                   const unsigned int random_movement_factor)
    : id(data.id),
      name(data.name),
      health_max(data.base_health),
      health(health),
      min_damage(data.min_damage),
      max_damage(data.max_damage),
      position(init_map, init_x_coord, init_y_coord, logic_maps),
      visible_range(data.visible_range),
      movement_speed(data.movement_speed),
      items(items),
      logic_maps(logic_maps),
      map_id(init_map),
      characters(characters),
      game(game),
      moving_cooldown(0),
      random_moving_cooldown(0),
      attribute_update_time_elapsed(0),
      attack_cooldown(data.attack_cooldown),
      actual_attack_cooldown(0),
      attacking_id(0),
      broadcast(false),
      formulas(formulas),
      rate(rate),
      random_movement_factor(random_movement_factor) {
    RandomNumberGenerator random_number_generator;
    level = random_number_generator((int)data.min_level, (int)data.max_level);
    posibles_orientations = {UP_ORIENTATION, DOWN_ORIENTATION, LEFT_ORIENTATION,
                             RIGHT_ORIENTATION};
}

Creature::~Creature() {}

InstanceId Creature::_getNearestCharacter() {
    InstanceId nearest_id = 0;
    unsigned int nearest_range = visible_range + 1;
    const unsigned int x_tile = position.getX();
    const unsigned int y_tile = position.getY();
    int abs_y = 0;
    int abs_x = 0;
    Map& map = logic_maps.getMap(map_id);
    for (int y = -(int)visible_range; y <= (int)visible_range; y++) {
        abs_y = std::abs(y);
        for (int x = -(int)visible_range + abs_y;
             x <= (int)visible_range - abs_y; x++) {
            InstanceId id = 0;
            abs_x = std::abs(x);
            try {
                id = map.getTile(x_tile + x, y_tile + y).occupant_id;
            } catch (Exception& e) {
                continue;
            }
            if (map.getTile(x_tile + x, y_tile + y).safe_zone) {
                continue;
            }
            if (characters.count(id) > 0) {
                if (abs_x + abs_y < int(nearest_range) &&
                    characters.at(id).getHealth() > 0) {
                    nearest_range = abs_x + abs_y;
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
    const int x = position.getX();
    const int y = position.getY();
    if (abs(position_character.getX() - x) >=
        abs(position_character.getY() - y)) {
        if (position_character.getX() > x) {
            if (logic_maps.isPositionValidForCreature(map_id, x + 1, y)) {
                position.changeOrientation(RIGHT_ORIENTATION);
                return;
            }
        } else {
            if (logic_maps.isPositionValidForCreature(map_id, x - 1, y)) {
                position.changeOrientation(LEFT_ORIENTATION);
                return;
            }
        }
    }
    if (position_character.getY() > y) {
        if (logic_maps.isPositionValidForCreature(map_id, x, y + 1)) {
            position.changeOrientation(DOWN_ORIENTATION);
            return;
        }

    } else {
        if (logic_maps.isPositionValidForCreature(map_id, x, y - 1)) {
            position.changeOrientation(UP_ORIENTATION);
            return;
        }
    }
    if (position_character.getX() > x) {
        position.changeOrientation(RIGHT_ORIENTATION);
    } else {
        position.changeOrientation(LEFT_ORIENTATION);
    }
}

void Creature::_updateDamage(const unsigned int it, const InstanceId id) {
    actual_attack_cooldown -= it * rate;
    attacking_id = 0;
    bool eluded;
    while (actual_attack_cooldown <= 0) {
        this->broadcast = true;
        RandomNumberGenerator random_number_generator;
        int damage = level * (int)random_number_generator(
                                 (int)this->min_damage, (int)this->max_damage);
        eluded = !characters.at(id).receiveAttack(damage, true);
        game.beAttackedByCreature(id, damage, eluded);
        actual_attack_cooldown += attack_cooldown;
        attacking_id = id;
    }
}

void Creature::_updateMovement(const unsigned int it) {
    this->moving_cooldown -= it * rate;

    while (this->moving_cooldown <= 0) {
        if (!this->position.move(true)) {
            moving_cooldown = 0;
            return;
        }

        this->broadcast = true;

        this->moving_cooldown += 1000 / movement_speed;
    }
}

void Creature::_setRandomOrientation() {
    RandomNumberGenerator gen;
    Orientation orientation =
        this->posibles_orientations[gen(0, posibles_orientations.size() - 1)];
    this->position.changeOrientation(orientation);
}

void Creature::_randomMovement(const unsigned int it) {
    this->random_moving_cooldown -= it * rate;

    while (random_moving_cooldown <= 0) {
        _setRandomOrientation();
        
        if (!this->position.move(true)) {
            random_moving_cooldown = 0;
            return;
        }

        this->broadcast = true;

        this->random_moving_cooldown +=
            random_movement_factor * 1000 / movement_speed;
    }
}

void Creature::act(const unsigned int it) {
    InstanceId id = _getNearestCharacter();
    if (!id) {
        attacking_id = 0;
        _randomMovement(it);
    } else {
        if (_attackNearstCharacter(characters.at(id).getPosition())) {
            _updateDamage(it, id);
        } else {
            _determinateDirectionAndMove(characters.at(id).getPosition());
            _updateMovement(it);
        }
    }
}

const bool Creature::receiveAttack(int& damage, const bool eludible) {
    health = std::max(0, (int)(health - damage));
    this->broadcast = true;
    return true;
}

const bool Creature::beAttacked() {
    return true;
}

const bool Creature::isNewbie() const {
    return false;
}

const bool Creature::recoverHealth(int& points) {
    throw CantRecoverCreaturesHealthException();
}

const Position& Creature::getPosition() const {
    return this->position;
}

void Creature::dropAllItems(std::vector<DroppingSlot>& dropped_items) {
    RandomNumberGenerator gen;

    switch (formulas.calculateDroppingItem()) {
        case DROP_NOTHING: {
            // fprintf(stderr, "DEBUG: Creature::drop: NOTHING\n");
            break;
        }

        case DROP_GOLD: {
            dropped_items.emplace_back(
                GOLD_BAG_ID,
                formulas.calculateGoldDroppingAmount(this->health_max));
            // fprintf(stderr, "DEBUG: Creature::drop: ORO. Item id: %i\n",
            //         GOLD_BAG_ID);
            break;
        }

        case DROP_POTION: {
            const std::vector<Id>& potions = this->items.getPotionsId();
            size_t potion_idx = gen((int)0, (int)potions.size() - 1);
            dropped_items.emplace_back(potions[potion_idx], 1);
            // fprintf(stderr, "DEBUG: Creature::drop: POTION. Item id: %i\n",
            //         potions[potion_idx]);
            break;
        }

        case DROP_WEAPON: {
            const std::vector<Id>& weapons = this->items.getWeaponsId();
            size_t weapon_idx = gen((int)0, (int)weapons.size() - 1);
            dropped_items.emplace_back(weapons[weapon_idx], 1);
            // fprintf(stderr, "DEBUG: Creature::drop: WEAPON. Item id: %i\n",
            //         weapons[weapon_idx]);
            break;
        }

        case DROP_WAND: {
            const std::vector<Id>& wands = this->items.getWandsId();
            size_t wand_idx = gen((int)0, (int)wands.size() - 1);
            dropped_items.emplace_back(wands[wand_idx], 1);
            // fprintf(stderr, "DEBUG: Creature::drop: WAND. Item id: %i\n",
            //         wands[wand_idx]);
            break;
        }

        case DROP_DEFENCE: {
            const std::vector<Id>& defences = this->items.getDefencesId();
            size_t defence_idx = gen((int)0, (int)defences.size() - 1);
            dropped_items.emplace_back(defences[defence_idx], 1);
            // fprintf(stderr, "DEBUG: Creature::drop: DEFENCE. Item id: %i\n",
            //         defences[defence_idx]);
            break;
        }
    }
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
    data.level = this->level;
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
