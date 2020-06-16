#include "../../../Common/includes/Exceptions/Exception.h"

#include "../includes/ItemsContainer.h"
#include "../includes/Config.h"

#include "../includes/Weapon.h"
#include "../includes/Wand.h"
#include "../includes/Defence.h"
#include "../includes/Potion.h"

ItemsContainer::ItemsContainer() {
    Config<WeaponCfg> weapons;
    Config<WandCfg> wands;
    Config<SpellCfg> spells;
    Config<DefenceCfg> defences;
    Config<PotionCfg> potions;

    const std::unordered_map<Id, WeaponCfg>& weapons_map = weapons.getMap();

    std::unordered_map<Id, WeaponCfg>::const_iterator it_weapons = weapons_map.begin();

    while (it_weapons != weapons_map.end()) {
        this->container.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(it_weapons->first),
            std::forward_as_tuple(new Weapon(it_weapons->second)));

        it_weapons++;
    }

    const std::unordered_map<Id, WandCfg>& wands_map = wands.getMap();

    std::unordered_map<Id, WandCfg>::const_iterator it_wands = wands_map.begin();

    while (it_wands != wands_map.end()) {
        this->container.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(it_wands->first),
            std::forward_as_tuple(new Wand(it_wands->second, spells[it_wands->second.spell_id])));

        it_wands++;
    }

    const std::unordered_map<Id, DefenceCfg>& defences_map = defences.getMap();

    std::unordered_map<Id, DefenceCfg>::const_iterator it_defences = defences_map.begin();

    while (it_defences != defences_map.end()) {
        this->container.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(it_defences->first),
            std::forward_as_tuple(new Defence(it_defences->second)));
        
        it_defences++;
    }

    const std::unordered_map<Id, PotionCfg>& potions_map = potions.getMap();

    std::unordered_map<Id, PotionCfg>::const_iterator it_potions = potions_map.begin();

    while (it_potions != potions_map.end()) {
        this->container.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(it_potions->first),
            std::forward_as_tuple(PotionFactory::newPotion(it_potions->second)));
        
        it_potions++;
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

Item* ItemsContainer::operator[](Id item_id) {
    if (!this->container.count(item_id))
        throw Exception("Trying to acces an invalid item ID.");

    return this->container.at(item_id);
}
