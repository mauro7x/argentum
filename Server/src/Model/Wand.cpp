#include "../../includes/Model/Wand.h"
#include "../../includes/Model/Character.h"

Wand::Wand(const WandCfg& wand_data, const SpellCfg& spell_data): 
    Wearable(wand_data.id, wand_data.name, 
            wand_data.price, WEAPON),
    spell(SpellFactory::newSpell(spell_data)) {}
    
Wand::~Wand() {
    delete spell;
}

const unsigned int Wand::use(Character& user) {
    user.doMagic();
    return spell->cast(user);
}

const unsigned int Wand::getRange() const {
    return this->spell->getRange();
}

const bool Wand::isHealing() const {
    return this->spell->isHealing();
}
