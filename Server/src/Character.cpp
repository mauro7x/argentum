#include <algorithm>
#include <math.h>

#include "../includes/Character.h"
#include "../includes/Formulas.h"

#include <iostream> //sacar

#define CRITICAL_ATTACKE_DAMAGE_MODIFIER 2

Character::Character(const RaceCfg& race, const KindCfg& kind,
                     const int id_map, const int init_x_coord, 
                     const int init_y_coord, MapContainer& map_container):
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
        position(id_map, init_x_coord, init_y_coord, map_container) {
    this->updateStatus(0); // Set max_health, max_mana
}

Character::~Character() {
    delete state;
}

void Character::updateStatus(const unsigned int seconds_elapsed) {
    this->max_health = Formulas::calculateMaxHealth(this->constitution,
                        this->kind.max_health_factor, 
                        this->race.max_health_factor, 
                        this->level.getLevel());
    this->max_mana = Formulas::calculateMaxMana(this->intelligence,
                        this->kind.max_mana_factor,
                        this->race.max_mana_factor,
                        this->level.getLevel());
    
    // ACTUALIZACIONES QUE DEPENDEN DEL TIEMPO -> MOVER A OTRA FUNCION
    // CUANDO ESTE IMPLEMENTADO EL FRAMERATE.
    unsigned int health_update = Formulas::calculateHealthTimeRecovery(
                                    this->race.health_recovery_factor,
                                    seconds_elapsed);
    unsigned int mana_update = Formulas::calculateManaTimeRecovery(
                                    this->race.mana_recovery_factor,
                                    seconds_elapsed);

    this->health = std::min(this->health + health_update, this->max_health);
    this->mana = std::min(this->mana + mana_update, this->max_mana);

    // Cuando sube de nivel [llevar a otra funcion]
    this->inventory.updateMaxAmountsOfGold();
}

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
}

void Character::recoverHealth(const unsigned int points) {
    // IMPLEMENTAR LOGICA RAZA/CLASE MAX_HEALTH
    this->health += points;
}

void Character::recoverMana(const unsigned int points) {
    // IMPLEMENTAR LOGICA RAZA/CLASE MAX_MANA
    this->mana += points;
}

void Character::consumeMana(const unsigned int points) {
    if (this->mana < points) {
        throw InsufficientManaException();
    }

    this->mana -= points;
}

const unsigned int Character::takeItem(Item* item) {
    return this->inventory.addItem(item);
}

Item* Character::dropItem(unsigned int position) {
    return this->inventory.gatherItem(position);
}

void Character::doMagic() {
    this->kind.doMagic();
}

void Character::beAttacked() {
    if (!this->state->canBeAttacked()) {
        throw StateOfCharacterCantBeAttackedException();
    }
}

/*
 * Efectua un ataque a otro jugador.
 * 
 * Si el arma tiene efecto sobre el jugador [e.g: es un baculo curativo],
 * i.e tiene rango cero, se usa inmediatamente.
 * 
 * Si el arma es de daño, se verifica si el otro jugador está dentro del
 * rango de dicha arma, y se efectua el ataque al recibir el atacado los puntos
 * de daño.
 * 
 * Lanza OutOfRangeAttackException si el otro jugador está fuera del rango del arma.
 *       TooHighLevelDifferenceOnAttackException si la diferencia de niveles es más
 * alta de la permitida para un ataque.
 *       NewbiesCantBeAttackedException si el jugador al que se quiere atacar es Newbie.
 *       InsufficientManaException si no puede usar el hechizo debido a déficit de maná.
 *       StateOfCharacterCantBeAttackedException si el jugador al que se quiere atacar
 * tiene un estado en el que no puede ser atacado.
 */
const unsigned int Character::attack(Character& attacked) {
    const unsigned int weapon_range = this->equipment.getAttackRange();
    // Si el arma no es de ataque, es curativa [range 0].
    if (weapon_range == 0) {
        return this->equipment.useAttackItem(*this);
    }

    // Notifico al estado del otro jugador que va a ser atacado.
    // Si no esta vivo, lanza excepción StateOfCharacterCantBeAttackedException
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
    const unsigned int distance = this->position.getDistance(
                                    attacked.getPosition());
    if (distance > weapon_range) {
        throw OutOfRangeAttackException();
    }

    // Está dentro del rango. Se define si el ataque es critico y se obtienen
    // los correspondientes puntos de daño. 
    bool critical_attack = Formulas::isCriticalAttack();
    unsigned int potential_damage = this->equipment.useAttackItem(*this);
    if (critical_attack) {
        potential_damage = potential_damage * CRITICAL_ATTACKE_DAMAGE_MODIFIER;
    }

    // El atacado recibe el daño del ataque.
    const unsigned int effective_damage = attacked.receiveAttack(
                                            potential_damage, critical_attack);
    
    // Actualizo exp.
    this->level.onAttackUpdate(effective_damage, attacked.getLevel());

    // Si murio, sumamos la exp. necesaria
    if (!attacked.getHealth()) {
        this->level.onKillUpdate(attacked.getMaxHealth(), attacked.getLevel());
    }

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
    damage_received = std::max((unsigned int) 0, damage - defense_points);

    this->health = std::max((unsigned int) 0, this->health - damage_received);

    // MORIR

    return damage_received;
}

void Character::die() {
    delete this->state;
    this->state = new Dead();
    // DROPEAR ORO EN EXCESO E ITEMS DEL INVENTARIO
}

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

const char* StateOfCharacterCantBeAttackedException::what() const noexcept {
    return "El jugador no puede ser atacado debido a su estado.";
}

void Character::debug() {
    std::cout << "**Character debug:**" << std::endl;
    this->inventory.debug();
    this->equipment.debug();
    std::cout << "health: " << this->health << std::endl;
    std::cout << "mana: " << this->mana << std::endl;
}
