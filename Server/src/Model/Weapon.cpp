#include "../../includes/Model/Weapon.h"

#include "../../../Common/includes/RandomNumberGenerator.h"
#include "../../includes/Model/Character.h"  // Evito dependencias circulares.

Weapon::Weapon(const WeaponCfg& data)
    : Wearable(data.id, data.name, data.price, WEAPON),
      attack_range(data.attack_range),
      min_damage(data.min_damage),
      max_damage(data.max_damage),
      cooldown(data.cooldown) {}
Weapon::~Weapon() {}

const unsigned int Weapon::use(Character& user) {
    user.setAttackCooldown(this->cooldown);
    
    RandomNumberGenerator random_number_generator;
    return random_number_generator(this->min_damage, this->max_damage);
}

const unsigned int Weapon::getRange() const {
    return this->attack_range;
}
