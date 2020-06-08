#include "../includes/Weapon.h"

Weapon::Weapon(WeaponCfg& data):
                    Wearable(data.id, data.name, data.price, 
                             data.type, data.min_damage, 
                             data.max_damage),
                             distant_attack(data.distant_attack) {}
Weapon::~Weapon() {}

const unsigned int Weapon::use() {
    // IMPLEMENTAR LOGICA DE ATAQUE.
    return getUsagePoints();
}
