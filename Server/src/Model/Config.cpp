#include "../../includes/Model/Config.h"

#include <array>
//-----------------------------------------------------------------------------
// API Pública

template <>
void Config<RaceCfg>::_parseFile() {
    json j = JSON::loadJsonFile(paths::config(RACES_CONFIG_FILEPATH));

    int size = j.size();
    for (int i = 0; i < size; i++) {
        RaceCfg race;

        race.id = j[i]["id"];
        race.name = j[i]["name"];
        j[i]["head_ids"].get_to(race.head_ids);
        j[i]["body_ids"].get_to(race.body_ids);
        race.dead_head_id = j[i]["dead_head_id"];
        race.dead_body_id = j[i]["dead_body_id"];

        race.max_health_factor = j[i]["max_health_factor"];
        race.max_mana_factor = j[i]["max_mana_factor"];
        race.health_recovery_factor = j[i]["health_recovery_factor"];
        race.mana_recovery_factor = j[i]["mana_recovery_factor"];
        race.intelligence = j[i]["intelligence"];
        race.constitution = j[i]["constitution"];
        race.strength = j[i]["strength"];
        race.agility = j[i]["agility"];
        race.is_shorter = j[i]["is_shorter"];

        config[race.id] = race;
    }
}

template <>
void Config<KindCfg>::_parseFile() {
    json j = JSON::loadJsonFile(paths::config(KINDS_CONFIG_FILEPATH));

    int size = j.size();
    for (int i = 0; i < size; i++) {
        KindCfg kind;

        kind.id = j[i]["id"];
        kind.name = j[i]["name"];
        kind.max_health_factor = j[i]["max_health_factor"];
        kind.max_mana_factor = j[i]["max_mana_factor"];
        kind.meditation_factor = j[i]["meditation_factor"];
        kind.intelligence = j[i]["intelligence"];
        kind.constitution = j[i]["constitution"];
        kind.strength = j[i]["strength"];
        kind.agility = j[i]["agility"];

        config[kind.id] = kind;
    }
}

template <>
void Config<NPCCfg>::_parseFile() {
    json j = JSON::loadJsonFile(paths::config(NPC_CONFIG_FILEPATH));

    int size = j.size();
    for (int i = 0; i < size; i++) {
        NPCCfg npc;

        npc.id = j[i]["id"];
        npc.name = j[i]["name"];
        j[i]["sellable_items"].get_to(npc.sellable_items);
        config[npc.id] = npc;
    }
}

template <>
void Config<WeaponCfg>::_parseFile() {
    json j = JSON::loadJsonFile(paths::config(ITEMS_CONFIG_FILEPATH));

    int size = j["weapons"].size();
    for (int i = 0; i < size; i++) {
        WeaponCfg weapon;

        weapon.id = j["weapons"][i]["id"];
        weapon.name = j["weapons"][i]["name"];
        weapon.price = j["weapons"][i]["price"];
        weapon.range = j["weapons"][i]["range"];
        weapon.min_damage = j["weapons"][i]["min_damage"];
        weapon.max_damage = j["weapons"][i]["max_damage"];
        weapon.cooldown = j["weapons"][i]["cooldown"];
        weapon.weapon_type = j["weapons"][i]["weapon_type"];

        config[weapon.id] = weapon;
    }
}

template <>
void Config<WandCfg>::_parseFile() {
    json j = JSON::loadJsonFile(paths::config(ITEMS_CONFIG_FILEPATH));

    int size = j["wands"].size();
    for (int i = 0; i < size; i++) {
        WandCfg wand;

        wand.id = j["wands"][i]["id"];
        wand.name = j["wands"][i]["name"];
        wand.spell_id = j["wands"][i]["spell_id"];
        wand.price = j["wands"][i]["price"];

        config[wand.id] = wand;
    }
}

template <>
void Config<SpellCfg>::_parseFile() {
    json j = JSON::loadJsonFile(paths::config(ITEMS_CONFIG_FILEPATH));

    int size = j["spells"].size();
    for (int i = 0; i < size; i++) {
        SpellCfg spell;

        spell.id = j["spells"][i]["id"];
        spell.name = j["spells"][i]["name"];
        spell.mana_usage_cost = j["spells"][i]["mana_usage_cost"];
        spell.range = j["spells"][i]["range"];
        spell.min_damage = j["spells"][i]["min_damage"];
        spell.max_damage = j["spells"][i]["max_damage"];
        spell.recovery_points = j["spells"][i]["recovery_points"];
        spell.cooldown = j["spells"][i]["cooldown"];
        spell.weapon_type = j["spells"][i]["weapon_type"];

        config[spell.id] = spell;
    }
}

template <>
void Config<DefenceCfg>::_parseFile() {
    json j = JSON::loadJsonFile(paths::config(ITEMS_CONFIG_FILEPATH));

    std::array<std::string, AMOUNT_OF_DEFFENCE_TYPES> defences = {
        "helmets", "armours", "shields"};

    std::array<WearableType, AMOUNT_OF_DEFFENCE_TYPES> defence_types = {
        HELMET, ARMOUR, SHIELD};

    for (int k = 0; k < AMOUNT_OF_DEFFENCE_TYPES; ++k) {
        int size = j[defences[k]].size();

        for (int i = 0; i < size; i++) {
            DefenceCfg defence;

            defence.id = j[defences[k]][i]["id"];
            defence.name = j[defences[k]][i]["name"];
            defence.type = defence_types[k];
            defence.min_defence = j[defences[k]][i]["min_defence"];
            defence.max_defence = j[defences[k]][i]["max_defence"];
            defence.price = j[defences[k]][i]["price"];

            config[defence.id] = defence;
        }
    }
}

template <>
void Config<PotionCfg>::_parseFile() {
    json j = JSON::loadJsonFile(paths::config(ITEMS_CONFIG_FILEPATH));

    int size = j["potions"].size();
    for (int i = 0; i < size; i++) {
        PotionCfg potion;

        potion.id = j["potions"][i]["id"];
        potion.name = j["potions"][i]["name"];
        potion.price = j["potions"][i]["price"];
        potion.type = j["potions"][i]["potion_type"];
        potion.recovery_points = j["potions"][i]["recovery_points"];

        config[potion.id] = potion;
    }
}

template <>
void Config<OtherItemsCfg>::_parseFile() {
    json j = JSON::loadJsonFile(paths::config(ITEMS_CONFIG_FILEPATH));

    int size = j["others"].size();
    for (int i = 0; i < size; i++) {
        OtherItemsCfg item;

        item.id = j["others"][i]["id"];
        item.name = j["others"][i]["name"];

        config[item.id] = item;
    }
}

template <>
void Config<CreatureCfg>::_parseFile() {
    json j = JSON::loadJsonFile(paths::config(CREATURES_CONFIG_FILEPATH));

    int size = j.size();
    for (int i = 0; i < size; i++) {
        CreatureCfg creature;

        creature.id = j[i]["id"];
        creature.name = j[i]["name"];
        creature.min_level = j[i]["min_level"];
        creature.max_level = j[i]["max_level"];
        creature.min_damage = j[i]["min_damage"];
        creature.max_damage = j[i]["max_damage"];
        creature.base_health = j[i]["base_health"];
        creature.visible_range = j[i]["visible_range"];
        creature.movement_speed = j[i]["movement_speed"];
        creature.attack_cooldown = j[i]["attack_cooldown"];

        config[creature.id] = creature;
    }
}

/* Para cualquier clase que no sea esperada, nos quejamos */
template <class T>
void Config<T>::_parseFile() {
    throw Exception("Unknown configuration requested.");
}

//-----------------------------------------------------------------------------
