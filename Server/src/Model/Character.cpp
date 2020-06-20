#include <math.h>
#include <algorithm>
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
                     const int init_y_coord)
    :

      health(kind.initial_health + race.initial_health),
      mana(kind.initial_mana + race.initial_mana),
      intelligence(kind.intelligence + race.intelligence),
      constitution(kind.constitution + race.constitution),
      strength(kind.strength + race.strength),
      agility(kind.agility + race.agility),

      race(race),
      kind(kind),

      state(new Alive()),

      inventory(this->level),

      position(init_map, init_x_coord, init_y_coord, map_container),

      moving_orientation(DEFAULT_MOVING_ORIENTATION),
      moving(false),
      moving_time_elapsed(0),
      attribute_update_time_elapsed(0),

      broadcast(true) {  // Cuando se crea, debe ser broadcasteado.

    // SI EL JUGADOR ESTA PERSISTIDO Y NO ES NUEVO, LLENAR TODO LO QUE SE TIENE
    // QUE LLENAR.

    this->updateLevelDependantAttributes();  // Set max_health, max_mana,
}  // max_inventory_gold.

Character::~Character() {
    delete state;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void Character::act(const unsigned int it) {
    if (moving)
        _updateMovement(it);

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

        this->recoverHealth(health_update);
        this->recoverMana(mana_update);

        attribute_update_time_elapsed -= TIME_TO_UPDATE_ATTRIBUTES;
    }
}

void Character::_updateMovement(const unsigned int it) {
    this->moving_time_elapsed += it * RATE;  // Tiempo en ms acum sin moverme.

    while (this->moving_time_elapsed >= TIME_TO_MOVE_A_TILE) {
        this->position.move(moving_orientation);

        this->broadcast = true;

        this->moving_time_elapsed -= TIME_TO_MOVE_A_TILE;
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  Movimiento
//-----------------------------------------------------------------------------

void Character::startMovingUp() {
    this->moving_orientation = UP_ORIENTATION;
    this->moving = true;
}

void Character::startMovingDown() {
    this->moving_orientation = DOWN_ORIENTATION;
    this->moving = true;
}

void Character::startMovingRight() {
    this->moving_orientation = RIGHT_ORIENTATION;
    this->moving = true;
}

void Character::startMovingLeft() {
    this->moving_orientation = LEFT_ORIENTATION;
    this->moving = true;
}

void Character::stopMoving() {
    this->moving = false;
    this->moving_time_elapsed = 0;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void Character::recoverHealth(const unsigned int points) {
    if (!points)
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
    Item* item_to_equip = this->inventory.gatherItem(inventory_position);

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

const unsigned int Character::takeItem(Item* item) {
    const unsigned int position = this->inventory.addItem(item);
    this->broadcast = true;
    return position;
}

Item* Character::dropItem(unsigned int position) {
    Item* dropped_item = this->inventory.gatherItem(position);
    this->broadcast = true;
    return dropped_item;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void Character::beAttacked() {
    if (!this->state->canBeAttacked()) {
        throw ActualStateCantBeAttackedException();
    }
}

void Character::doMagic() {
    if (!this->kind.canDoMagic()) {
        throw KindCantDoMagicException();
    }
}

const unsigned int Character::attack(Character& attacked) {
    const unsigned int weapon_range = this->equipment.getAttackRange();

    // Si el arma no es de ataque, es curativa [range 0].
    if (weapon_range == 0) {
        return this->equipment.useAttackItem(*this);
    }

    // Verifico si dado el estado del otro jugador puedo atacarlo.
    attacked.beAttacked();

    // Verificacion de diferencia de niveles entre jugadores y newbie.
    if (attacked.isNewbie()) {
        throw NewbiesCantBeAttackedException();
    }

    if (!(Formulas::canAttackByLevel(this->level.getLevel(),
                                     attacked.getLevel()))) {
        throw TooHighLevelDifferenceOnAttackException();
    }

    // Se trata de un ataque de daño.
    // Nos fijamos si el atacado está en el rango del arma.
    // Si no lo está, lanzamos excepción.
    const unsigned int distance =
        this->position.getDistance(attacked.getPosition());
    if (distance > weapon_range) {
        throw OutOfRangeAttackException();
    }

    // Está dentro del rango. Se define si el ataque es critico y se obtienen
    // los correspondientes puntos de daño.
    bool critical_attack = Formulas::isCriticalAttack();
    unsigned int potential_damage = this->equipment.useAttackItem(*this);
    if (critical_attack) {
        potential_damage = potential_damage * CRITICAL_ATTACK_DAMAGE_MODIFIER;
    }

    // El atacado recibe el daño del ataque.
    const unsigned int effective_damage =
        attacked.receiveAttack(potential_damage, critical_attack);

    // Actualizo exp.
    this->level.onAttackUpdate(*this, effective_damage, attacked.getLevel());

    // Si murio, sumamos la exp. necesaria
    if (!attacked.getHealth()) {
        this->level.onKillUpdate(*this, attacked.getMaxHealth(),
                                 attacked.getLevel());
    }

    this->broadcast = true;

    return effective_damage;
}

const unsigned int Character::receiveAttack(const unsigned int damage,
                                            const bool eludible) {
    unsigned int damage_received = 0;

    if (eludible) {
        const bool is_eluded = Formulas::isAttackEluded(this->agility);
        if (is_eluded) {
            // El ataque es esquivado, no se recibe daño
            return damage_received;
        }
    }

    const unsigned int defense_points = this->equipment.getDefensePoints(*this);
    damage_received = std::max((unsigned int)0, damage - defense_points);

    this->health = std::max((unsigned int)0, this->health - damage_received);

    if (this->health == 0) {
        // MORIR
        this->die();
    }

    this->broadcast = true;

    return damage_received;
}

void Character::die() {
    delete this->state;
    this->state = new Dead();
    // DROPEAR ORO EN EXCESO E ITEMS DEL INVENTARIO
    this->broadcast = true;
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
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
const bool Character::mustBeBroadcasted() const {
    return this->broadcast;
}

void Character::beBroadcasted() {
    this->broadcast = false;
}

void Character::fillBroadcastData(PlayerData& data) const {
    // Llena x_tile, y_tile, orientation.
    this->position.fillBroadcastData(data);

    data.head_id = this->race.head_id;
    data.body_id = this->race.body_id;

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

const char* OutOfRangeAttackException::what() const noexcept {
    return "El jugador al que quieres atacar está fuera del rango de tu arma.";
}

const char* NewbiesCantBeAttackedException::what() const noexcept {
    return "No puedes atacar a jugador newbie.";
}

const char* TooHighLevelDifferenceOnAttackException::what() const noexcept {
    return "No puedes atacar. La diferencia de niveles es mayor a 12.";
}

const char* ActualStateCantBeAttackedException::what() const noexcept {
    return "El jugador no puede ser atacado debido a su estado.";
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
