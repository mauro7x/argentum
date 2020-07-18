#include "../../includes/Model/Wand.h"

#include "../../includes/Model/Character.h"

Wand::Wand(const WandCfg& wand_data, const SpellCfg& spell_data)
    : Wearable(wand_data.id, wand_data.name, wand_data.price, WEAPON),
      spell(SpellFactory::newSpell(spell_data)) {}

Wand::~Wand() {
    delete spell;
}

Response Wand::use(Character& user, int& points) {
    Response response = user.doMagic();
    if (!response.succeeded)
        return response;

    return spell->cast(user, points);
}

const unsigned int Wand::getRange() const {
    return this->spell->getRange();
}

const WeaponType Wand::getWeaponType() const {
    return this->spell->getWeaponType();
}
