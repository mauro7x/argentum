#include "../includes/Character.h"

#include <iostream> //sacar

//IMPLEMENTAR

Character::Character(const RaceCfg& race, 
                     const KindCfg& kind): 
                        race(race),
                        kind(kind),
                        state(new Alive()) {
    this->mana = 100; // Pasar a JSON!
    this->health = 100; // Pasar a JSON!
}
Character::~Character() {
    delete state;
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
