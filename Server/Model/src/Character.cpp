#include <algorithm>
#include <math.h>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/UnitData.h"
#include "../includes/Character.h"
#include "../includes/Formulas.h"
//-----------------------------------------------------------------------------
#include <iostream> //sacar
//-----------------------------------------------------------------------------
#define CRITICAL_ATTACK_DAMAGE_MODIFIER 2
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Character::Character(const PlayerData& init_data,
                     const RaceCfg& race, const KindCfg& kind,
                     MapContainer& map_container):
        
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

        position(init_data.basic_data.map_id,
                 init_data.basic_data.x_tile,
                 init_data.basic_data.y_tile,
                 map_container) {
                     
    // SI EL JUGADOR ESTA PERSISTIDO Y NO ES NUEVO, LLENAR TODO LO QUE SE TIENE QUE LLENAR.
    
    this->updateLevelDependantAttributes(); // Set max_health, max_mana,
}                                           // max_inventory_gold.


Character::~Character() {
    delete state;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void Character::updateLevelDependantAttributes() {
    this->max_health = Formulas::calculateMaxHealth(this->constitution,
                        this->kind.max_health_factor, 
                        this->race.max_health_factor, 
                        this->level.getLevel());
    this->max_mana = Formulas::calculateMaxMana(this->intelligence,
                        this->kind.max_mana_factor,
                        this->race.max_mana_factor,
                        this->level.getLevel());

    this->inventory.updateMaxAmountsOfGold();
}

void Character::updateTimeDependantAttributes(const unsigned int seconds_elapsed) {
    // ACTUALIZACIONES QUE DEPENDEN DEL TIEMPO
    unsigned int health_update = Formulas::calculateHealthTimeRecovery(
                                    this->race.health_recovery_factor,
                                    seconds_elapsed);
    unsigned int mana_update = Formulas::calculateManaTimeRecovery(
                                    this->race.mana_recovery_factor,
                                    seconds_elapsed);

    this->recoverHealth(health_update);
    this->recoverMana(mana_update);

    // IMPLEMENTAR LOGICA DE MOVIMIENTO, QUE DEPENDE DEL TIEMPO.
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void Character::recoverHealth(const unsigned int points) {
    this->health = std::min(this->health + points, max_health);
}

void Character::recoverMana(const unsigned int points) {
    this->mana = std::min(this->mana + points, max_mana);
}

void Character::consumeMana(const unsigned int points) {
    if (this->mana < points) {
        throw InsufficientManaException();
    }

    this->mana -= points;
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
}

const unsigned int Character::takeItem(Item* item) {
    return this->inventory.addItem(item);
}

Item* Character::dropItem(unsigned int position) {
    return this->inventory.gatherItem(position);
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
        potential_damage = potential_damage * CRITICAL_ATTACK_DAMAGE_MODIFIER;
    }

    // El atacado recibe el daño del ataque.
    const unsigned int effective_damage = attacked.receiveAttack(
                                            potential_damage, critical_attack);
    
    // Actualizo exp.
    this->level.onAttackUpdate(*this, effective_damage, attacked.getLevel());

    // Si murio, sumamos la exp. necesaria
    if (!attacked.getHealth()) {
        this->level.onKillUpdate(*this, attacked.getMaxHealth(), attacked.getLevel());
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

    if (this->health == 0) {
        // MORIR
        this->die();
    }

    return damage_received;
}

void Character::die() {
    delete this->state;
    this->state = new Dead();
    // DROPEAR ORO EN EXCESO E ITEMS DEL INVENTARIO
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
    this->inventory.debug();
    this->equipment.debug();
    std::cout << "health: " << this->health << std::endl;
    std::cout << "mana: " << this->mana << std::endl;
}
//-----------------------------------------------------------------------------
