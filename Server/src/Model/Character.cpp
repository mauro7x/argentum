#include <math.h>

#include <algorithm>
#include <string>
//-----------------------------------------------------------------------------
#include "../../includes/Model/Character.h"
#include "../../includes/Model/Formulas.h"
//-----------------------------------------------------------------------------
#include <iostream>  //sacar
//-----------------------------------------------------------------------------
#define DEFAULT_MOVING_ORIENTATION DOWN_ORIENTATION
//-----------------------------------------------------------------------------
#define NO_ITEM_TO_EQUIP_ERROR_MSG "No tienes ningún item en esa posición."
#define COLLISION_WHILE_MOVING_ERROR_MSG "No puedes moverte en esa dirección."
#define KIND_CANT_DO_MAGIC_ERROR_MSG "Tu clase no puede hacer magia."
#define KIND_CANT_MEDITATE_ERROR_MSG "Tu clase no puede meditar."
#define STATE_CANT_MOVE_ERROR_MSG "No puedes moverte. Estás resucitando."
#define STATE_CANT_TAKE_ITEM_ERROR_MSG "No puedes tomar items. Estás muerto."
#define STATE_CANT_BE_HEALED_ERROR_MSG "No puedes curarte. Estás muerto."
#define STATE_CANT_MEDITATE_ERROR_MSG "No puedes meditar. Estás muerto."
#define STATE_CANT_RESURRECT_ERROR_MSG "No puedes resucitar. Ya estás vivo."
#define STATE_CANT_GATHER_GOLD_ERROR_MSG "No puedes usar tu oro. Estás muerto."
#define STATE_CANT_ATTACK_ERROR_MSG "No puedes atacar. Estás muerto."
#define STATE_CANT_BE_ATTACKED_ERROR_MSG \
    "No puedes atacar a ese jugador. Ya está muerto"
//-----------------------------------------------------------------------------
#define CANT_ATTACK_IN_SAFE_ZONE_ERROR_MSG \
    "No se puede atacar o lanzar hechizos en una zona segura."
#define CANT_ATTACK_ITSELF_ERROR_MSG "No puedes hacerte daño a ti mismo."
#define OUT_OF_RANGE_ATTACK_ERROR_MSG \
    "El objetivo al que quieres atacar está fuera del rango de tu arma."
#define NEWBIES_CANT_BE_ATTACKED_ERROR_MSG "No puedes atacar a jugador newbie."
#define NEWBIES_CANT_ATTACK_CHARACTERS_ERROR_MSG \
    "No puedes atacar a otro jugador. Eres newbie."
#define ATTACK_COOLDOWN_TIME_NOT_ELAPSED_ERROR_MSG \
    "No puedes usar el arma tan rápido, la misma está en enfriamiento."
#define TOO_HIGH_LEVEL_DIFFERENCE_ON_ATTACK_ERROR_MSG \
    "La diferencia de niveles es mayor a 12."
//-----------------------------------------------------------------------------
#define HEALED_SUCCESS_MSG "Has sido curado."
#define MEDITATING_SUCCESS_MSG \
    "Has comenzado a meditar. Ante cualquier acción dejarás de hacerlo."
//-----------------------------------------------------------------------------

Character::Character(const CharacterCfg& init_data, const RaceCfg& race,
                     const KindCfg& kind, LogicMaps& logic_maps,
                     const Id init_map, const int init_x_coord,
                     const int init_y_coord, ItemsContainer& items_container,
                     Formulas& formulas, const int& rate,
                     const unsigned int critical_attack_dmg_modifier,
                     const unsigned int ms_to_update_attributes)
    : intelligence(kind.intelligence + race.intelligence),
      constitution(kind.constitution + race.constitution),
      strength(kind.strength + race.strength),
      agility(kind.agility + race.agility),

      nickname(init_data.nickname),

      race(race, init_data.head_id, init_data.body_id),
      kind(kind),

      state(StateFactory::newState(init_data.state, this->race)),

      level(init_data.level, init_data.exp, formulas),

      inventory(init_data.inventory, init_data.safe_gold, init_data.excess_gold,
                this->level, items_container, formulas),
      equipment(init_data.equipment, items_container),

      position(init_map, init_x_coord, init_y_coord, logic_maps),

      is_meditating(false),

      is_moving(false),
      moving_cooldown(0),
      attribute_update_time_elapsed(0),

      attack_cooldown(0),
      broadcast(false),

      formulas(formulas),

      rate(rate),
      critical_attack_dmg_modifier(critical_attack_dmg_modifier),
      ms_to_update_attributes(ms_to_update_attributes) {
    this->updateLevelDependantAttributes();  // Set max_health, max_mana,
                                             // max_inventory_gold.
    if (init_data.new_created) {
        health = max_health;
        mana = max_mana;
    } else {
        health = init_data.health;
        mana = init_data.mana;
    }
}

Character::~Character() {
    delete state;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Actualización de atributos
//-----------------------------------------------------------------------------

Response Character::act(const unsigned int it) {
    if (attack_cooldown > 0)
        attack_cooldown = std::max((int)(attack_cooldown - it * rate), 0);

    _updateTimeDependantAttributes(it);

    if (is_moving)
        return _updateMovement(it);

    // Si no se está moviendo, igual actualizamos el cooldown.
    // (Si se está moviendo se actualiza en _updateMovement()).
    if (moving_cooldown > 0)
        moving_cooldown = std::max((int)(moving_cooldown - it * rate), 0);

    return Response(true, "", SUCCESS_MSG);
}

void Character::updateLevelDependantAttributes() {
    unsigned int health_diference, mana_diference;
    health_diference = this->max_health - this->health;
    mana_diference = this->max_mana - this->mana;
    this->max_health = formulas.calculateMaxHealth(
        this->constitution, this->kind.max_health_factor,
        this->race.max_health_factor, this->level.getLevel());
    this->health = this->max_health - health_diference;

    this->max_mana = formulas.calculateMaxMana(
        this->intelligence, this->kind.max_mana_factor,
        this->race.max_mana_factor, this->level.getLevel());
    this->mana = this->max_mana - mana_diference;
    this->inventory.updateMaxAmountsOfGold();

    this->broadcast = true;
}

void Character::_updateTimeDependantAttributes(const unsigned int it) {
    attribute_update_time_elapsed += it * rate;

    while (attribute_update_time_elapsed >= ms_to_update_attributes) {
        int health_update = formulas.calculateHealthTimeRecovery(
            this->race.health_recovery_factor, ms_to_update_attributes);

        unsigned int mana_update = formulas.calculateManaTimeRecovery(
            this->race.mana_recovery_factor, ms_to_update_attributes);

        if (this->is_meditating)
            mana_update += formulas.calculateManaMeditationTimeRecovery(
                this->kind.meditation_factor, this->intelligence,
                ms_to_update_attributes);

        if ((this->health < this->max_health))
            this->recoverHealth(health_update);
        if ((this->mana < this->max_mana))
            this->recoverMana(mana_update);

        attribute_update_time_elapsed -= ms_to_update_attributes;
    }
}

Response Character::_updateMovement(const unsigned int it) {
    this->moving_cooldown -= it * rate;

    while (this->moving_cooldown <= 0) {
        this->broadcast = true;

        this->moving_cooldown += UNIT_TIME_TO_MOVE;

        if (!this->position.move(false)) {
            this->stopMoving();
            return Response(false, COLLISION_WHILE_MOVING_ERROR_MSG, ERROR_MSG);
        }
    }

    return Response(true, "", SUCCESS_MSG);
}

std::string Character::getNickname() {
    return nickname;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Movimiento
//-----------------------------------------------------------------------------

Response Character::startMovingUp() {
    if (!this->state->move())
        return Response(false, STATE_CANT_MOVE_ERROR_MSG, ERROR_MSG);

    this->is_meditating = false;
    this->position.changeOrientation(UP_ORIENTATION);
    this->is_moving = true;

    return Response(true, "", SUCCESS_MSG);
}

Response Character::startMovingDown() {
    if (!this->state->move())
        return Response(false, STATE_CANT_MOVE_ERROR_MSG, ERROR_MSG);

    this->is_meditating = false;
    this->position.changeOrientation(DOWN_ORIENTATION);
    this->is_moving = true;

    return Response(true, "", SUCCESS_MSG);
}

Response Character::startMovingRight() {
    if (!this->state->move())
        return Response(false, STATE_CANT_MOVE_ERROR_MSG, ERROR_MSG);

    this->is_meditating = false;
    this->position.changeOrientation(RIGHT_ORIENTATION);
    this->is_moving = true;

    return Response(true, "", SUCCESS_MSG);
}

Response Character::startMovingLeft() {
    if (!this->state->move())
        return Response(false, STATE_CANT_MOVE_ERROR_MSG, ERROR_MSG);

    this->is_meditating = false;
    this->position.changeOrientation(LEFT_ORIENTATION);
    this->is_moving = true;

    return Response(true, "", SUCCESS_MSG);
}

void Character::stopMoving() {
    this->is_moving = false;
}

void Character::teleport(const Id map_id, const uint32_t x_coord,
                         const uint32_t y_coord) {
    this->position.teleport(map_id, x_coord, y_coord);
    this->broadcast = true;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Modificación de maná y vida.
//-----------------------------------------------------------------------------

const bool Character::recoverHealth(int& points) {
    if (!points || !health || this->health == this->max_health) {
        points = 0;
        return false;
    }

    if (health + points > max_health) {
        points = max_health - health;
        health = max_health;
    } else {
        health += points;
    }

    this->broadcast = true;
    return true;
}

const bool Character::recoverMana(unsigned int& points) {
    if (!points || !health || this->mana == this->max_mana)
        return false;

    if (mana + points > max_mana) {
        points = max_mana - mana;
        mana = max_mana;
    } else {
        mana += points;
    }

    this->broadcast = true;
    return true;
}

Response Character::heal() {
    if (!this->state->beHealed())
        return Response(false, STATE_CANT_BE_HEALED_ERROR_MSG, ERROR_MSG);

    this->health = max_health;
    this->mana = max_mana;
    this->broadcast = true;

    return Response(true, HEALED_SUCCESS_MSG, SUCCESS_MSG);
}

const bool Character::consumeMana(const unsigned int points) {
    if (this->mana < points)
        return false;

    this->mana -= points;
    this->broadcast = true;
    return true;
}

Response Character::meditate() {
    if (!this->state->meditate())
        return Response(false, STATE_CANT_MEDITATE_ERROR_MSG, ERROR_MSG);

    if (!this->kind.meditate())
        return Response(false, KIND_CANT_MEDITATE_ERROR_MSG, ERROR_MSG);

    this->is_meditating = true;

    return Response(true, MEDITATING_SUCCESS_MSG, SUCCESS_MSG);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Manejo de items.
//-----------------------------------------------------------------------------

Response Character::equip(unsigned int inventory_position) {
    unsigned int amount = 1;
    Item* item_to_equip =
        this->inventory.gatherItem(inventory_position, amount);

    if (!item_to_equip)
        return Response(false, NO_ITEM_TO_EQUIP_ERROR_MSG, ERROR_MSG);

    this->broadcast = true;

    return item_to_equip->equip(*this);
}

Response Character::equip(Wearable* item) {
    Wearable* prev_equipped_item = this->equipment.add(item);
    if (prev_equipped_item) {
        // Agrego lo que tenia equipado al inventario.
        takeItem(prev_equipped_item);
    }

    this->broadcast = true;
    return Response(true, "", SUCCESS_MSG);
}

Response Character::unequip(unsigned int n_slot) {
    Wearable* unequipped_item = this->equipment.remove(n_slot);

    if (!unequipped_item)
        return Response(true, "", SUCCESS_MSG);

    Response response = this->inventory.addItem((Item*)unequipped_item, 1);

    if (!response.succeeded) {
        // No se pudo agregar al inventario => lo devolvemos al equipment
        this->equipment.add(unequipped_item);
    } else {
        this->broadcast = true;
    }

    return response;
}

Response Character::takeItem(Item* item, unsigned int amount) {
    if (!this->state->takeItem())
        return Response(false, STATE_CANT_TAKE_ITEM_ERROR_MSG, ERROR_MSG);

    this->broadcast = true;
    return this->inventory.addItem(item, amount);
}

Item* Character::dropItem(const unsigned int n_slot, unsigned int& amount) {
    Item* dropped_item = this->inventory.gatherItem(n_slot, amount);

    if (dropped_item)
        this->broadcast = true;

    return dropped_item;
}

Response Character::gatherGold(const unsigned int amount) {
    if (!this->state->gatherGold())
        return Response(false, STATE_CANT_GATHER_GOLD_ERROR_MSG, ERROR_MSG);

    this->broadcast = true;
    return this->inventory.gatherGold(amount);
}

Response Character::takeGold(unsigned int& amount) {
    if (!this->state->takeItem())
        return Response(false, STATE_CANT_TAKE_ITEM_ERROR_MSG, ERROR_MSG);

    this->broadcast = true;
    return this->inventory.addGold(amount);
}

void Character::dropAllItems(std::vector<DroppingSlot>& dropped_items) {
    this->equipment.dropAll(dropped_items);
    this->inventory.dropAll(dropped_items);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Ataque y defensa.
//-----------------------------------------------------------------------------

void Character::setAttackCooldown(const unsigned int cooldown) {
    this->attack_cooldown += cooldown;
}

const bool Character::beAttacked() {
    // Delego en mi estado si puedo ser atacado.
    return this->state->beAttacked();
}

Response Character::doMagic() const {
    // Delego en mi kind si puedo hacer magia.
    if (!this->kind.doMagic())
        return Response(false, KIND_CANT_DO_MAGIC_ERROR_MSG, ERROR_MSG);

    return Response(true, "", SUCCESS_MSG);
}

Response Character::_checkPriorToUseWeaponConditions(Attackable* target) const {
    // Delego si puedo atacar en mi estado.
    if (!this->state->attack())
        return Response(false, STATE_CANT_ATTACK_ERROR_MSG, ERROR_MSG);

    // Verifico cooldown de arma de ataque.
    if (this->attack_cooldown)
        return Response(false, ATTACK_COOLDOWN_TIME_NOT_ELAPSED_ERROR_MSG,
                        ERROR_MSG);

    // Verifico si alguno atacante o atacado está en zona segura.
    if (this->position.isInSafeZone() || target->getPosition().isInSafeZone())
        return Response(false, CANT_ATTACK_IN_SAFE_ZONE_ERROR_MSG, ERROR_MSG);

    // Nos fijamos si el atacado está en el rango del arma.
    // Si no lo está, lanzamos excepción.
    const unsigned int weapon_range = this->equipment.getAttackRange();
    const unsigned int distance =
        this->position.getDistance(target->getPosition());

    if (distance > weapon_range)
        return Response(false, OUT_OF_RANGE_ATTACK_ERROR_MSG, ERROR_MSG);

    return Response(true, "", SUCCESS_MSG);
}

Response Character::_checkPriorToUseAttackWeaponConditions(
    Attackable* target) const {
    // Verificación de que no se quiera hacer daño a sí mismo
    if (this == target)
        return Response(false, CANT_ATTACK_ITSELF_ERROR_MSG, ERROR_MSG);

    // Verificación si dado el estado del otro jugador se puede atacarlo.
    if (!target->beAttacked())
        return Response(false, STATE_CANT_BE_ATTACKED_ERROR_MSG, ERROR_MSG);

    // Verificación de si el jugador es newbie.
    if (target->isNewbie())
        return Response(false, NEWBIES_CANT_BE_ATTACKED_ERROR_MSG, ERROR_MSG);

    if (this->isNewbie() && !target->isCreature())
        return Response(false, NEWBIES_CANT_ATTACK_CHARACTERS_ERROR_MSG,
                        ERROR_MSG);

    // Verificación de diferencia de niveles entre jugadores.
    const unsigned int target_level = target->getLevel();

    if (!target->isCreature() &&
        !(formulas.canAttackByLevel(this->level.getLevel(), target_level)))
        return Response(false, TOO_HIGH_LEVEL_DIFFERENCE_ON_ATTACK_ERROR_MSG,
                        ERROR_MSG);

    return Response(true, "", SUCCESS_MSG);
}

Response Character::_useHealingWeapon(Attackable* target, int& damage) {
    // Si el arma es curativa, obtengo sus puntos y curo.
    int healing_points;

    Response response = this->equipment.useAttackItem(*this, healing_points);
    if (!response.succeeded)
        return response;

    target->recoverHealth(healing_points);

    // Establezco valor negativo de daño -> puntos curativos.
    damage = -healing_points;
    return Response(true, "", SUCCESS_MSG);
}

Response Character::_useAttackWeapon(Attackable* target, int& damage,
                                     bool& eluded) {
    Response checks_response = _checkPriorToUseAttackWeaponConditions(target);
    if (!checks_response.succeeded)
        return checks_response;

    // Obtenemos el danio del arma.
    Response usage_response = this->equipment.useAttackItem(*this, damage);
    if (!usage_response.succeeded)
        return usage_response;

    // Se define si el ataque es critico
    bool critical_attack = formulas.isCriticalAttack();
    if (critical_attack)
        damage = damage * critical_attack_dmg_modifier;

    // El atacado recibe el daño del ataque.
    eluded = !target->receiveAttack(damage, critical_attack);

    // Sumamos exp. de ataque.
    this->level.onAttackUpdate(*this, damage, target->getLevel());

    // Si murió, sumamos la exp. de muerte.
    if (!target->getHealth())
        this->level.onKillUpdate(*this, target->getMaxHealth(),
                                 target->getLevel());

    this->broadcast = true;

    return Response(true, "", SUCCESS_MSG);
}

Response Character::useWeapon(Attackable* target, int& damage, bool& eluded) {
    Response response = _checkPriorToUseWeaponConditions(target);
    if (!response.succeeded)
        return response;

    this->is_meditating = false;

    if (this->equipment.getWeaponType() == HEALING) {
        eluded = false;
        return _useHealingWeapon(target, damage);
    }

    // Es un arma de ataque.
    return _useAttackWeapon(target, damage, eluded);
}

const bool Character::receiveAttack(int& damage, const bool eludible) {
    this->is_meditating = false;

    if (eludible && formulas.isAttackEluded(this->agility)) {
        // El ataque es esquivado, no se recibe daño
        damage = 0;
        return false;
    }

    const unsigned int defense_points = this->equipment.getDefensePoints(*this);

    damage = std::max(0, (int)(damage - defense_points));

    this->health = std::max(0, (int)(this->health - damage));

    if (this->health == 0)
        this->die();

    this->broadcast = true;

    return true;
}

void Character::die() {
    delete this->state;
    this->state = new Dead(this->race.dead_head_id, this->race.dead_body_id);

    this->mana = 0;

    this->broadcast = true;
}

Response Character::resurrect() {
    if (!this->state->resurrect())
        return Response(false, STATE_CANT_RESURRECT_ERROR_MSG, ERROR_MSG);

    delete this->state;
    this->state = new Alive(this->race.head_id, this->race.body_id);

    this->heal();
    this->broadcast = true;

    return Response(true, "", SUCCESS_MSG);
}

Response Character::startResurrecting() {
    if (!this->state->resurrect())
        return Response(false, STATE_CANT_RESURRECT_ERROR_MSG, ERROR_MSG);

    this->stopMoving();

    delete this->state;
    this->state =
        new Resurrecting(this->race.dead_head_id, this->race.dead_body_id);

    return Response(true, "", SUCCESS_MSG);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Obtención de estado.
//-----------------------------------------------------------------------------

const Position& Character::getPosition() const {
    return this->position;
}

const unsigned int Character::getLevel() const {
    return this->level.getLevel();
}

const unsigned int Character::getHealth() const {
    return this->health;
}

const unsigned int Character::getMaxHealth() const {
    return this->max_health;
}

const bool Character::isNewbie() const {
    return this->level.isNewbie();
}

const Id Character::getMapId() const {
    return this->position.getMapId();
}

const std::string& Character::getNickname() const {
    return this->nickname;
}

const bool Character::isCreature() const {
    return false;
}

const WeaponType Character::getWeaponType() const {
    return this->equipment.getWeaponType();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Control de broadcast.
//-----------------------------------------------------------------------------

const bool Character::mustBeBroadcasted() const {
    return this->broadcast;
}

void Character::beBroadcasted() {
    this->broadcast = false;
    this->position.beBroadcasted();
}

void Character::fillBroadcastData(PlayerData& data) const {
    // Llena map_id, x_tile, y_tile, orientation.
    this->position.fillBroadcastData(data.basic_data);
    this->state->fillBroadcastData(data);

    data.basic_data.movement_speed = UNIT_MOVEMENT_SPEED;
    data.is_shorter = race.is_shorter;
    data.nickname = this->nickname;
    data.health = this->health;
    data.max_health = this->max_health;
    data.mana = this->mana;
    data.max_mana = this->max_mana;

    // Llena level, exp, levelup_exp
    this->level.fillBroadcastData(data);
    // Llena safe_gold, excess_gold e inventory.
    this->inventory.fillBroadcastData(data);
    // Llena equipment, helmet_id, armour_id, shield_id, weapon_id;
    this->equipment.fillBroadcastData(data);
}

void Character::fillPersistenceData(CharacterCfg& data) const {
    // llenar map x_tile, y_tile;
    this->position.fillPersistenceData(data);

    data.head_id = this->race.head_id;
    data.body_id = this->race.body_id;

    std::strncpy(data.nickname, nickname.c_str(), sizeof(data.nickname) - 1);

    data.race = this->race.id;
    data.kind = this->kind.id;

    // aca verifica el estado del character, hay que mejorar!
    if (!this->health) {
        data.state = DEAD;
    } else {
        data.state = ALIVE;
    }

    this->level.fillPersistenceData(data);
    this->equipment.fillPersistenceData(data);
    this->inventory.fillPersistenceData(data);

    data.health = this->health;
    data.mana = this->mana;
    data.new_created = false;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
