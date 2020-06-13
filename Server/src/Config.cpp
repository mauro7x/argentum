#include <array>

#include "../includes/Config.h"
//-----------------------------------------------------------------------------
// API Pública

template <>
void Config<RaceCfg>::_parseFile() {
    json j = JSON::loadJsonFile(RACES_CONFIG_FILEPATH);

    int size = j.size();
    for (int i = 0; i < size; i++) {
        RaceCfg race;

        race.id = j[i]["id"];
        race.name = j[i]["name"];
        race.max_health_factor = j[i]["health"];
        race.health_recovery_factor = j[i]["recovery"];
        race.max_mana_factor = j[i]["mana"];
        race.intelligence = j[i]["intelligence"];
        race.constitution = j[i]["constitution"];
        race.strength = j[i]["strength"];
        race.agility = j[i]["agility"];

        config[race.id] = race;
    }
}

template <>
void Config<KindCfg>::_parseFile() {
    json j = JSON::loadJsonFile(KINDS_CONFIG_FILEPATH);

    int size = j.size();
    for (int i = 0; i < size; i++) {
        KindCfg kind;

        kind.id = j[i]["id"];
        kind.name = j[i]["name"];
        kind.max_health_factor = j[i]["max_health_factor"];
        kind.meditation_factor = j[i]["meditation_factor"];
        kind.max_mana_factor = j[i]["max_mana_factor"];
        kind.initial_mana = j[i]["initial_mana"];
        kind.initial_health = j[i]["initial_health"];

        config[kind.id] = kind;
    }
}

template <>
void Config<NPCCfg>::_parseFile() {
    json j = JSON::loadJsonFile(CREATURES_CONFIG_FILEPATH);

    int size = j.size();
    for (int i = 0; i < size; i++) {
        NPCCfg npc;

        npc.id = j[i]["id"];
        npc.name = j[i]["name"];
        config[npc.id] = npc;
    }
}

template <>
void Config<WeaponCfg>::_parseFile() {
    json j = JSON::loadJsonFile(ITEMS_CONFIG_FILEPATH);

    int size = j["weapons"].size();
    for (int i = 0; i < size; i++) {
        WeaponCfg weapon;

        weapon.id = j["weapons"][i]["id"];
        weapon.name = j["weapons"][i]["name"];
        weapon.price = j["weapons"][i]["price"];
        weapon.attack_distance = j["weapons"][i]["attack_distance"];
        weapon.min_damage = j["weapons"][i]["min_damage"];
        weapon.max_damage = j["weapons"][i]["max_damage"];

        config[weapon.id] = weapon;
    }
}

template <>
void Config<WandCfg>::_parseFile() {
    json j = JSON::loadJsonFile(ITEMS_CONFIG_FILEPATH);

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
    json j = JSON::loadJsonFile(ITEMS_CONFIG_FILEPATH);

    int size = j["spells"].size();
    for (int i = 0; i < size; i++) {
        SpellCfg spell;

        spell.id = j["spells"][i]["id"];
        spell.name = j["spells"][i]["name"];
        spell.type = j["spells"][i]["spell_type"];
        spell.mana_usage_cost = j["spells"][i]["mana_usage_cost"];
        spell.attack_distance = j["spells"][i]["attack_distance"];
        spell.min_damage = j["spells"][i]["min_damage"];
        spell.max_damage = j["spells"][i]["max_damage"];
        spell.recovery_points = j["spells"][i]["recovery_points"];

        config[spell.id] = spell;
    }
}

template <>
void Config<DefenceCfg>::_parseFile() {
    json j = JSON::loadJsonFile(ITEMS_CONFIG_FILEPATH);

    std::array<std::string, AMOUNT_OF_DEFFENCE_TYPES> defences = 
        {"helmets", "armours", "shields"};
    
    std::array<WearableType, AMOUNT_OF_DEFFENCE_TYPES> defence_types =
        {HELMET, ARMOUR, SHIELD};

    for (int k = 0; k < AMOUNT_OF_DEFFENCE_TYPES;
         ++k) {
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
    json j = JSON::loadJsonFile(ITEMS_CONFIG_FILEPATH);

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



/* Para cualquier clase que no sea esperada, nos quejamos */
template <class T>
void Config<T>::_parseFile() {
    throw Exception("Unknown configuration requested.");
}

//-----------------------------------------------------------------------------
