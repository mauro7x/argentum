#include "../includes/Wand.h"


Wand::Wand(const WandCfg& wand_data, const SpellCfg& spell_data): 
    Wearable(wand_data.id, wand_data.name, 
            wand_data.price, WEAPON),
    spell(SpellFactory::newSpell(spell_data)) {}
Wand::~Wand() {
    delete spell;
}

const unsigned int Wand::use(Character& user) {
    return spell->cast(user);
}
