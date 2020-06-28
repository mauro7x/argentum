#include <math.h>

#include <algorithm>
#include <string>
//-----------------------------------------------------------------------------
#include "../../includes/Model/Character.h"
#include "../../includes/Model/Formulas.h"
//-----------------------------------------------------------------------------
#include <iostream>  //sacar
//-----------------------------------------------------------------------------
#define CRITICAL_ATTACK_DAMAGE_MODIFIER 2
#define RATE 1000 / 30                  // ms.
#define TIME_TO_MOVE_A_TILE 200         // ms
#define TIME_TO_UPDATE_ATTRIBUTES 1000  // ms

#define DEFAULT_MOVING_ORIENTATION DOWN_ORIENTATION
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Character::Character(const CharacterCfg& init_data, const RaceCfg& race,
                     const KindCfg& kind, MapContainer& map_container,
                     const Id init_map, const int init_x_coord,
                     const int init_y_coord, ItemsContainer& items_container)
    :

      health(init_data.health),
      mana(init_data.mana),

      intelligence(kind.intelligence + race.intelligence),
      constitution(kind.constitution + race.constitution),
      strength(kind.strength + race.strength),
      agility(kind.agility + race.agility),

      race(race),
      kind(kind),

      state(StateFactory::newState(init_data.state, this->race)),

      level(init_data.level, init_data.exp),

      inventory(init_data.inventory, init_data.safe_gold, init_data.excess_gold,
                this->level, items_container),
      equipment(init_data.equipment, items_container),

      position(init_map, init_x_coord, init_y_coord, map_container),

      is_moving(false),
      moving_cooldown(0),
      attribute_update_time_elapsed(0),

      attack_cooldown(0),

      broadcast(false) {
    this->updateLevelDependantAttributes();  // Set max_health, max_mana,
                                             // max_inventory_gold.
}

Character::~Character() {
    delete state;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void Character::act(const unsigned int it) {
    if (is_moving) {
        _updateMovement(it);
    } else {
        if (moving_cooldown > 0) {
            moving_cooldown = std::max(moving_cooldown - RATE, 0);
        }
    }

    if (attack_cooldown > 0)
        attack_cooldown = std::max(attack_cooldown - RATE, 0);

    _updateTimeDependantAttributes(it);
}

void Character::updateLevelDependantAttributes() {
    this->max_health = Formulas::calculateMaxHealth(
        this->constitution, this->kind.max_health_factor,
        this->race.max_health_factor, this->level.getLevel());

    this->max_mana = Formulas::calculateMaxMana(
        this->intelligence, this->kind.max_mana_factor,
        this->race.max_mana_factor, this->level.getLevel());

    this->inventory.updateMaxAmountsOfGold();

    this->broadcast = true;
}

void Character::_updateTimeDependantAttributes(const unsigned int it) {
    attribute_update_time_elapsed += it * RATE;

    while (attribute_update_time_elapsed >= TIME_TO_UPDATE_ATTRIBUTES) {
        unsigned int health_update = Formulas::calculateHealthTimeRecovery(
            this->race.health_recovery_factor,
            TIME_TO_UPDATE_ATTRIBUTES / 1000);  // en segundos

        unsigned int mana_update = Formulas::calculateManaTimeRecovery(
            this->race.mana_recovery_factor,
            TIME_TO_UPDATE_ATTRIBUTES / 1000);  // en segundos

        if ((this->health < this->max_health))
            this->recoverHealth(health_update);
        if ((this->mana < this->max_mana))
            this->recoverMana(mana_update);

        attribute_update_time_elapsed -= TIME_TO_UPDATE_ATTRIBUTES;
    }
}

void Character::_updateMovement(const unsigned int it) {
    this->moving_cooldown -= it * RATE;

    while (this->moving_cooldown <= 0) {
        this->broadcast = true;

        this->position.move();

        this->moving_cooldown += TIME_TO_MOVE_A_TILE;
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Movimiento
//-----------------------------------------------------------------------------

void Character::startMovingUp() {
    this->position.changeOrientation(UP_ORIENTATION);
    this->is_moving = true;
}

void Character::startMovingDown() {
    this->position.changeOrientation(DOWN_ORIENTATION);
    this->is_moving = true;
}

void Character::startMovingRight() {
    this->position.changeOrientation(RIGHT_ORIENTATION);
    this->is_moving = true;
}

void Character::startMovingLeft() {
    this->position.changeOrientation(LEFT_ORIENTATION);
    this->is_moving = true;
}

void Character::stopMoving() {
    this->is_moving = false;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void Character::recoverHealth(const unsigned int points) {
    if (!points || !health)
        return;

    this->health = std::min(this->health + points, max_health);
    this->broadcast = true;
}

void Character::recoverMana(const unsigned int points) {
    if (!points)
        return;

    this->mana = std::min(this->mana + points, max_mana);
    this->broadcast = true;
}

void Character::consumeMana(const unsigned int points) {
    if (this->mana < points) {
        throw InsufficientManaException();
    }

    this->mana -= points;
    this->broadcast = true;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void Character::equip(unsigned int inventory_position) {
    unsigned int amount = 1;
    Item* item_to_equip =
        this->inventory.gatherItem(inventory_position, amount);

    if (!item_to_equip)
        return;

    item_to_equip->equip(*this);
}

void Character::equip(Wearable* item) {
    Wearable* prev_equipped_item = this->equipment.add(item);
    if (prev_equipped_item) {
        // Agrego lo que tenia equipado al inventario.
        takeItem(prev_equipped_item);
    }

    this->broadcast = true;
}

void Character::unequip(unsigned int n_slot) {
    Wearable* unequipped_item = this->equipment.remove(n_slot);

    if (!unequipped_item)
        return;

    try {
        this->inventory.addItem((Item*)unequipped_item, 1);
    } catch (FullInventoryException()) {
        // No se pudo agregar al inventario => lo devolvemos al equipment
        this->equipment.add(unequipped_item);
        throw FullInventoryException();
    }

    this->broadcast = true;
}

void Character::takeItem(Item* item, unsigned int amount) {
    this->state->takeItem();
    this->inventory.addItem(item, amount);
    this->broadcast = true;
}

Item* Character::dropItem(const unsigned int n_slot, unsigned int& amount) {
    Item* dropped_item = this->inventory.gatherItem(n_slot, amount);

    if (dropped_item)
        this->broadcast = true;

    return dropped_item;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

void Character::setAttackCooldown(const unsigned int cooldown) {
    this->attack_cooldown += cooldown;
}

void Character::beAttacked() {
    // Delego en mi estado si puedo ser atacado.
    this->state->beAttacked();
}

void Character::doMagic() {
    if (!this->kind.canDoMagic()) {
        throw KindCantDoMagicException();
    }
}

const bool Character::attack(Character& attacked, int& damage) {
    // Delego si puedo atacar en mi estado.
    this->state->attack();

    // Verifico si tiene arma de ataque.
    if (!this->equipment.hasAWeaponEquipped())
        throw CantAttackWithoutWeaponException();

    // Verifico cooldown de arma de ataque.
    if (this->attack_cooldown)
        throw AttackCooldownTimeNotElapsedException();

    // Verifico si alguno atacante o atacado está en zona segura.
    if (this->position.isInSafeZone() || attacked.getPosition().isInSafeZone())
        throw CantAttackInSafeZoneException();

    const unsigned int weapon_range = this->equipment.getAttackRange();

    // Si el arma es curativa, obtengo sus puntos y curo.
    if (this->equipment.isWeaponHealing()) {
        int healing_points = this->equipment.useAttackItem(*this);
        attacked.recoverHealth(healing_points);

        // Devuelvo valor negativo -> puntos curativos.
        damage = -healing_points;
        return false;
    }

    // Es un arma de ataque.

    // Verifico que no se quiera hacer daño a sí mismo
    if (this == &attacked)
        throw CantAttackItselfException();

    // Verifico si dado el estado del otro jugador puedo atacarlo.
    attacked.beAttacked();

    // Verificacion de diferencia de niveles entre jugadores y newbie.
    if (attacked.isNewbie()) {
        throw NewbiesCantBeAttackedException();
    }

    if (!(Formulas::canAttackByLevel(this->level.getLevel(),
                                     attacked.getLevel())))
        throw TooHighLevelDifferenceOnAttackException();

    // Se trata de un ataque de daño.
    // Nos fijamos si el atacado está en el rango del arma.
    // Si no lo está, lanzamos excepción.
    const unsigned int distance =
        this->position.getDistance(attacked.getPosition());

    if (distance > weapon_range)
        throw OutOfRangeAttackException();

    // Está dentro del rango. Se define si el ataque es critico y se obtienen
    // los correspondientes puntos de daño.
    damage = this->equipment.useAttackItem(*this);

    bool critical_attack = Formulas::isCriticalAttack();
    if (critical_attack)
        damage = damage * CRITICAL_ATTACK_DAMAGE_MODIFIER;

    // El atacado recibe el daño del ataque.
    const bool eluded = attacked.receiveAttack(damage, critical_attack);

    // Actualizo exp.
    this->level.onAttackUpdate(*this, damage, attacked.getLevel());

    // Si murio, sumamos la exp. necesaria
    if (!attacked.getHealth())
        this->level.onKillUpdate(*this, attacked.getMaxHealth(),
                                 attacked.getLevel());

    this->broadcast = true;

    return eluded;
}

const bool Character::receiveAttack(int& damage, const bool eludible) {
    if (eludible && Formulas::isAttackEluded(this->agility)) {
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
    this->state = new Dead(this->race);
    this->broadcast = true;
}

void Character::dropAllItems(std::vector<DroppingSlot>& dropped_items) {
    this->equipment.dropAll(dropped_items);
    this->inventory.dropAll(dropped_items);
}
//-----------------------------------------------------------------------------

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
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
const bool Character::mustBeBroadcasted() const {
    return this->broadcast;
}

void Character::beBroadcasted() {
    this->broadcast = false;
}

void Character::fillBroadcastData(PlayerData& data) const {
    // Llena map_id, x_tile, y_tile, orientation.
    this->position.fillBroadcastData(data.basic_data);

    this->state->fillBroadcastData(data);

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
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
const char* InsufficientManaException::what() const noexcept {
    return "No tienes suficiente maná.";
}

const char* CantAttackInSafeZoneException::what() const noexcept {
    return "No se puede atacar en una zona segura.";
}

const char* CantAttackItselfException::what() const noexcept {
    return "No puedes hacerte daño a ti mismo.";
}

const char* OutOfRangeAttackException::what() const noexcept {
    return "El jugador al que quieres atacar está fuera del rango de tu arma.";
}

const char* CantAttackWithoutWeaponException::what() const noexcept {
    return "No puedes atacar sin arma.";
}

const char* NewbiesCantBeAttackedException::what() const noexcept {
    return "No puedes atacar a jugador newbie.";
}

const char* AttackCooldownTimeNotElapsedException::what() const noexcept {
    return "No puedes usar el arma todavía. Cooldown.";
}

const char* TooHighLevelDifferenceOnAttackException::what() const noexcept {
    return "No puedes atacar. La diferencia de niveles es mayor a 12.";
}

const char* KindCantDoMagicException::what() const noexcept {
    return "Tu clase no puede lanzar hechizos.";
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void Character::debug() {
    std::cout << "**Character debug:**" << std::endl;
    // this->inventory.debug();
    // this->equipment.debug();
    std::cout << "health: " << this->health << std::endl;
    std::cout << "mana: " << this->mana << std::endl;
    std::cout << "position: x = " << this->position.getX()
              << " y = " << this->position.getY() << std::endl;
}
//-----------------------------------------------------------------------------
