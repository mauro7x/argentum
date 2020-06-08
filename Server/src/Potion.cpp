#include "../includes/Potion.h"
#include "../includes/Character.h"

Potion::Potion(const unsigned int id,
               std::string name,
               const unsigned int price,
               const unsigned int recovery_points):
                    Item(id, name, price),
                    recovery_points(recovery_points) {}

Potion::~Potion() {}

HealthPotion::HealthPotion(const unsigned int id,
                           const std::string name,
                           const unsigned int price,
                           const unsigned int recovery_points):
                                Potion(id, name, price, recovery_points) {}
HealthPotion::~HealthPotion() {}

void HealthPotion::equip(Character& character) {
    character.recoverHealth(this->recovery_points);
}

ManaPotion::ManaPotion(const unsigned int id, 
                       const std::string name,
                       const unsigned int price,
                       const unsigned int recovery_points):
                            Potion(id, name, price, recovery_points) {}
ManaPotion::~ManaPotion() {}

void ManaPotion::equip(Character& character) {
    character.recoverMana(this->recovery_points);
}
