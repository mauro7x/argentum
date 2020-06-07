#include "Weapon.h"

Weapon::Weapon(const unsigned int id, const unsigned int price, 
               WearableType type,
               const unsigned int min_damage, 
               const unsigned int max_damage,
               const bool distant_attack):
                    Wearable(id, price, WEAPON, min_damage, max_damage),
                    distant_attack(distant_attack) {}
Weapon::~Weapon() {}

const unsigned int Weapon::use() {
    // IMPLEMENTAR LOGICA DE ATAQUE.
    return getUsagePoints();
}
