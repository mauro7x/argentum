#include "Weapon.h"


Weapon::Weapon(const unsigned int id, const unsigned int buy_price,
               const unsigned int sell_price, const bool distant_attack, 
               const unsigned int min_damage, const unsigned int max_damage):
                    Item(id, buy_price, sell_price),
                    distant_attack(distant_attack),
                    min_damage(min_damage),
                    max_damage(max_damage) {}
Weapon::~Weapon() {}

void Weapon::use(Character& character) {
    
}

const unsigned int Wand::getMinDamagePoints() const {
    return this->min_damage;
}
const unsigned int Wand::getMaxDamagePoints() const {
    return this->max_damage;
}
