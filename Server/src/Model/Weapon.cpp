#include "../../includes/Model/Weapon.h"
//-----------------------------------------------------------------------------
#include "../../../Common/includes/RandomNumberGenerator.h"
#include "../../includes/Model/Character.h"  // Evito dependencias circulares.
//-----------------------------------------------------------------------------
Weapon::Weapon(const WeaponCfg& data)
    : Wearable(data.id, data.name, data.price, WEAPON),
      range(data.range),
      min_damage(data.min_damage),
      max_damage(data.max_damage),
      cooldown(data.cooldown),
      weapon_type(data.weapon_type) {}
Weapon::~Weapon() {}

Response Weapon::use(Character& user, int& points) {
    user.setAttackCooldown(this->cooldown);

    RandomNumberGenerator random_number_generator;
    points = (int)random_number_generator((int)this->min_damage,
                                        (int)this->max_damage);
    return Response(true, "", SUCCESS_MSG);
}

const unsigned int Weapon::getRange() const {
    return this->range;
}

const WeaponType Weapon::getWeaponType() const {
    return this->weapon_type;
}
