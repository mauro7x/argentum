#include "Potion.h"

Potion::Potion(const unsigned int id, const unsigned int buy_price, 
               const unsigned int sell_price, 
               const unsigned int recovery_points):
                    Item(id, buy_price, sell_price),
                    recovery_points(recovery_points) {}

void Potion::equip(Character& character) {
    this->use(character);
}

HealthPotion::HealthPotion(const unsigned int id, const unsigned int buy_price, 
                           const unsigned int sell_price, 
                           const unsigned int recovery_points):
                                Potion(id, buy_price, 
                                       sell_price, 
                                       recovery_points) {}
HealthPotion::~HealthPotion() {}

void HealthPotion::use(Character& character) {
    character.recoverHealth(this->recovery_points);
}

ManaPotion::ManaPotion(const unsigned int id, const unsigned int buy_price, 
                           const unsigned int sell_price, 
                           const unsigned int recovery_points):
                                Potion(id, buy_price, 
                                       sell_price, 
                                       recovery_points) {}
ManaPotion::~ManaPotion() {}

void ManaPotion::use(Character& character) {
    character.recoverMana(this->recovery_points);
}
