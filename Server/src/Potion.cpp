#include "Potion.h"

Potion::Potion(const unsigned int id,
               const unsigned int price,
               const unsigned int recovery_points):
                    Item(id, price),
                    recovery_points(recovery_points) {}

HealthPotion::HealthPotion(const unsigned int id,
                           const unsigned int price,
                           const unsigned int recovery_points):
                                Potion(id, price, recovery_points) {}
HealthPotion::~HealthPotion() {}

void HealthPotion::equip(Character& character) {
    character.recoverHealth(this->recovery_points);
}

ManaPotion::ManaPotion(const unsigned int id, 
                       const unsigned int price,
                       const unsigned int recovery_points):
                            Potion(id, price, recovery_points) {}
ManaPotion::~ManaPotion() {}

void ManaPotion::equip(Character& character) {
    character.recoverMana(this->recovery_points);
}
