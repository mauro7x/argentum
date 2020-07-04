#include "../../includes/Model/ItemsContainer.h"

#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../includes/Model/Config.h"
#include "../../includes/Model/Defence.h"
#include "../../includes/Model/GoldBag.h"
#include "../../includes/Model/Potion.h"
#include "../../includes/Model/Wand.h"
#include "../../includes/Model/Weapon.h"

ItemsContainer::ItemsContainer() {
    Config<WeaponCfg> weapons;
    Config<WandCfg> wands;
    Config<SpellCfg> spells;
    Config<DefenceCfg> defences;
    Config<PotionCfg> potions;
    Config<OtherItemsCfg> others;

    weapons.gatherIds(weapons_id);
    size_t fist_idx = 0;

    for (unsigned int i = 0; i < weapons_id.size(); ++i) {
        Id id = weapons_id[i];
        this->container.emplace(std::piecewise_construct,
                                std::forward_as_tuple(id),
                                std::forward_as_tuple(new Weapon(weapons[id])));
        if (container[id]->what() == "Puño")
            fist_idx = i;
    }

    weapons_id.erase(weapons_id.begin() + fist_idx);

    wands.gatherIds(wands_id);

    for (unsigned int i = 0; i < wands_id.size(); ++i) {
        Id id = wands_id[i];
        this->container.emplace(std::piecewise_construct,
                                std::forward_as_tuple(id),
                                std::forward_as_tuple(new Wand(
                                    wands[id], spells[wands[id].spell_id])));
    }

    defences.gatherIds(defences_id);

    for (unsigned int i = 0; i < defences_id.size(); ++i) {
        Id id = defences_id[i];
        this->container.emplace(
            std::piecewise_construct, std::forward_as_tuple(id),
            std::forward_as_tuple(new Defence(defences[id])));
    }

    potions.gatherIds(potions_id);

    for (unsigned int i = 0; i < potions_id.size(); ++i) {
        Id id = potions_id[i];
        this->container.emplace(
            std::piecewise_construct, std::forward_as_tuple(id),
            std::forward_as_tuple(PotionFactory::newPotion(potions[id])));
    }

    std::vector<Id> others_id;
    others.gatherIds(others_id);

    this->container.emplace(
        std::piecewise_construct, std::forward_as_tuple(GOLD_BAG_ID),
        std::forward_as_tuple(new GoldBag(others[GOLD_BAG_ID])));
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

const std::vector<Id>& ItemsContainer::getWeaponsId() const {
    return this->weapons_id;
}

const std::vector<Id>& ItemsContainer::getWandsId() const {
    return this->wands_id;
}

const std::vector<Id>& ItemsContainer::getDefencesId() const {
    return this->defences_id;
}

const std::vector<Id>& ItemsContainer::getPotionsId() const {
    return this->potions_id;
}
