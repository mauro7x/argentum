#include <algorithm>

#include "../includes/Character.h"

#include <iostream> //sacar

Character::Character(const RaceCfg& race, 
                     const KindCfg& kind):
        health(kind.initial_health + race.initial_health),
        mana(kind.initial_mana + race.initial_mana),
        intelligence(kind.intelligence + race.intelligence),
        constitution(kind.constitution + race.constitution),
        strength(kind.strength + race.strength),
        agility(kind.agility + race.agility),
        race(race),
        kind(kind),
        state(new Alive()) {
    this->updateStatus(0); // Set max_health, max_mana
}

Character::~Character() {
    delete state;
}

void Character::updateStatus(const unsigned int seconds_elapsed) {
    this->max_health = this->constitution * this->kind.max_health_factor * 
                       this->race.max_health_factor * this->level.getLevel();
    this->max_mana = this->intelligence * this->kind.max_mana_factor * 
                     this->race.max_mana_factor * this->level.getLevel();
    
    unsigned int health_update = this->race.health_recovery_factor * seconds_elapsed;
    unsigned int mana_update = this->race.mana_recovery_factor * seconds_elapsed;

    this->health = std::min(this->health + health_update, this->max_health);
    this->mana = std::min(this->mana + mana_update, this->max_mana);
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
    if (this->mana < points)
        throw InsufficientManaException();
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

const char* InsufficientManaException::what() const noexcept {
    return "No tienes suficiente maná.";
}

void Character::debug() {
    std::cout << "**Character debug:**" << std::endl;
    this->inventory.debug();
    this->equipment.debug();
    std::cout << "mana: " << this->mana << std::endl;
    std::cout << "Defense points: " << this->equipment.getDefensePoints(*this) << std::endl;
    std::cout << "Attack points: " << this->equipment.getAttackPoints(*this) << std::endl;
    std::cout << "health: " << this->health << std::endl;
    std::cout << "mana: " << this->mana << std::endl;
}
