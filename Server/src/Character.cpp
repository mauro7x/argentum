#include "../includes/Character.h"

//IMPLEMENTAR

Character::Character(RaceCfg& race, 
                     KindCfg& kind): 
                        race(race),
                        kind(kind),
                        state(new Alive()) {}
Character::~Character() {
    delete state;
}

void Character::equip(unsigned int inventory_position) {
    // IMPLEMENTAR
}

void Character::equip(Wearable* item) {
    // IMPLEMENTAR
}

void Character::recoverHealth(const unsigned int points) {
    // IMPLEMENTAR
}

void Character::recoverMana(const unsigned int points) {
    // IMPLEMENTAR
}

void Character::consumeMana(const unsigned int points) {
    if (this->mana < points)
        throw InsufficientManaException();
    this->mana -= points;
}

const char* InsufficientManaException::what() const noexcept {
    return "No tienes suficiente maná.";
}
