#include "../../includes/Model/ItemsContainer.h"

#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../includes/Model/Config.h"
#include "../../includes/Model/Defence.h"
#include "../../includes/Model/Potion.h"
#include "../../includes/Model/Wand.h"
#include "../../includes/Model/Weapon.h"

ItemsContainer::ItemsContainer() {
    Config<WeaponCfg> weapons;
    Config<WandCfg> wands;
    Config<SpellCfg> spells;
    Config<DefenceCfg> defences;
    Config<PotionCfg> potions;

    std::vector<Id> weapons_id;
    weapons.gatherIds(weapons_id);

    for (unsigned int i = 0; i < weapons_id.size(); ++i) {
        Id id = weapons_id[i];
        this->container.emplace(std::piecewise_construct,
                                std::forward_as_tuple(id),
                                std::forward_as_tuple(new Weapon(weapons[id])));
    }

    std::vector<Id> wands_id;
    wands.gatherIds(wands_id);

    for (unsigned int i = 0; i < wands_id.size(); ++i) {
        Id id = wands_id[i];
        this->container.emplace(std::piecewise_construct,
                                std::forward_as_tuple(id),
                                std::forward_as_tuple(new Wand(
                                    wands[id], spells[wands[id].spell_id])));
    }

    std::vector<Id> defences_id;
    defences.gatherIds(defences_id);

    for (unsigned int i = 0; i < defences_id.size(); ++i) {
        Id id = defences_id[i];
        this->container.emplace(
            std::piecewise_construct, std::forward_as_tuple(id),
            std::forward_as_tuple(new Defence(defences[id])));
    }

    std::vector<Id> potions_id;
    potions.gatherIds(potions_id);

    for (unsigned int i = 0; i < potions_id.size(); ++i) {
        Id id = potions_id[i];
        this->container.emplace(
            std::piecewise_construct, std::forward_as_tuple(id),
            std::forward_as_tuple(PotionFactory::newPotion(potions[id])));
    }
}

ItemsContainer::~ItemsContainer() {
    // Eliminamos todos los items.
    std::unordered_map<Id, Item*>::iterator it = this->container.begin();

    while (it != this->container.end()) {
        delete it->second;
        it++;
    }
}

Item* ItemsContainer::operator[](const Id item_id) {
    if (!this->container.count(item_id))
        throw Exception("Trying to acces an invalid item ID.");

    return this->container.at(item_id);
}
