#include <array>

#include "../includes/Config.h"
//-----------------------------------------------------------------------------
// API Pública

template <>
void Config<RaceCfg>::_parseFile() {
    std::ifstream file(RACE_CONFIG_FILEPATH);
    if (file.fail()) {
        throw Exception("Error opening file: %s", RACE_CONFIG_FILEPATH);
    }

    json j;
    file >> j;
    if (file.fail()) {
        throw Exception("Error reading file: %s", RACE_CONFIG_FILEPATH);
    }

    int size = j.size();
    for (int i = 0; i < size; i++) {
        RaceCfg race;

        race.id = j[i]["id"];
        race.name = j[i]["name"];
        race.health = j[i]["health"];
        race.recovery = j[i]["recovery"];
        race.mana = j[i]["mana"];

        config[race.id] = race;
    }

    file.close();
    if (file.fail()) {
        throw Exception("Error closing file: %s", RACE_CONFIG_FILEPATH);
    }
}

template <>
void Config<KindCfg>::_parseFile() {
    std::ifstream file(KIND_CONFIG_FILEPATH);
    if (file.fail()) {
        throw Exception("Error opening file: %s", KIND_CONFIG_FILEPATH);
    }

    json j;
    file >> j;
    if (file.fail()) {
        throw Exception("Error reading file: %s", KIND_CONFIG_FILEPATH);
    }

    int size = j.size();
    for (int i = 0; i < size; i++) {
        KindCfg kind;

        kind.id = j[i]["id"];
        kind.name = j[i]["name"];
        kind.health = j[i]["health"];
        kind.meditation = j[i]["meditation"];
        kind.mana = j[i]["mana"];

        config[kind.id] = kind;
    }

    file.close();
    if (file.fail()) {
        throw Exception("Error closing file: %s", KIND_CONFIG_FILEPATH);
    }
}

template <>
void Config<NPCCfg>::_parseFile() {
    std::ifstream file(NPC_CONFIG_FILEPATH);
    if (file.fail()) {
        throw Exception("Error opening file: %s", NPC_CONFIG_FILEPATH);
    }

    json j;
    file >> j;
    if (file.fail()) {
        throw Exception("Error reading file: %s", NPC_CONFIG_FILEPATH);
    }

    int size = j.size();
    for (int i = 0; i < size; i++) {
        NPCCfg npc;

        npc.id = j[i]["id"];
        npc.name = j[i]["name"];
        config[npc.id] = npc;
    }

    file.close();
    if (file.fail()) {
        throw Exception("Error closing file: %s", NPC_CONFIG_FILEPATH);
    }
}

template <>
void Config<WeaponCfg>::_parseFile() {
    std::ifstream file(ITEMS_CONFIG_FILEPATH);
    if (file.fail()) {
        throw Exception("Error opening file: %s", ITEMS_CONFIG_FILEPATH);
    }

    json j;
    file >> j;
    if (file.fail()) {
        throw Exception("Error reading file: %s", ITEMS_CONFIG_FILEPATH);
    }

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

    file.close();
    if (file.fail()) {
        throw Exception("Error closing file: %s", ITEMS_CONFIG_FILEPATH);
    }
}

template <>
void Config<WandCfg>::_parseFile() {
    std::ifstream file(ITEMS_CONFIG_FILEPATH);
    if (file.fail()) {
        throw Exception("Error opening file: %s", ITEMS_CONFIG_FILEPATH);
    }

    json j;
    file >> j;
    if (file.fail()) {
        throw Exception("Error reading file: %s", ITEMS_CONFIG_FILEPATH);
    }

    int size = j["wands"].size();
    for (int i = 0; i < size; i++) {
        WandCfg wand;

        wand.id = j["wands"][i]["id"];
        wand.name = j["wands"][i]["name"];
        wand.spell_id = j["wands"][i]["spell_id"];
        wand.price = j["wands"][i]["price"];

        config[wand.id] = wand;
    }

    file.close();
    if (file.fail()) {
        throw Exception("Error closing file: %s", ITEMS_CONFIG_FILEPATH);
    }
}

template <>
void Config<SpellCfg>::_parseFile() {
    std::ifstream file(ITEMS_CONFIG_FILEPATH);
    if (file.fail()) {
        throw Exception("Error opening file: %s", ITEMS_CONFIG_FILEPATH);
    }

    json j;
    file >> j;
    if (file.fail()) {
        throw Exception("Error reading file: %s", ITEMS_CONFIG_FILEPATH);
    }

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

    file.close();
    if (file.fail()) {
        throw Exception("Error closing file: %s", ITEMS_CONFIG_FILEPATH);
    }
}

template <>
void Config<DefenceCfg>::_parseFile() {
    std::ifstream file(ITEMS_CONFIG_FILEPATH);
    if (file.fail()) {
        throw Exception("Error opening file: %s", ITEMS_CONFIG_FILEPATH);
    }

    json j;
    file >> j;
    if (file.fail()) {
        throw Exception("Error reading file: %s", ITEMS_CONFIG_FILEPATH);
    }

    std::array<std::string, AMOUNT_OF_DEFFENCE_TYPES> defences = 
        {"helmets", "armours", "shields"}; 

    for (int k = 0; k < AMOUNT_OF_DEFFENCE_TYPES;
         ++k) {
        int size = j[defences[k]].size();

        for (int i = 0; i < size; i++) {
            DefenceCfg defence;

            defence.id = j[defences[k]][i]["id"];
            defence.name = j[defences[k]][i]["name"];
            defence.type = j[defences[k]][i]["wearable_type"];
            defence.min_defence = j[defences[k]][i]["min_defence"];
            defence.max_defence = j[defences[k]][i]["max_defence"];
            defence.price = j[defences[k]][i]["price"];

            config[defence.id] = defence;
        }
    }

    file.close();
    if (file.fail()) {
        throw Exception("Error closing file: %s", ITEMS_CONFIG_FILEPATH);
    }
}

template <>
void Config<PotionCfg>::_parseFile() {
    std::ifstream file(ITEMS_CONFIG_FILEPATH);
    if (file.fail()) {
        throw Exception("Error opening file: %s", ITEMS_CONFIG_FILEPATH);
    }

    json j;
    file >> j;
    if (file.fail()) {
        throw Exception("Error reading file: %s", ITEMS_CONFIG_FILEPATH);
    }

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

    file.close();
    if (file.fail()) {
        throw Exception("Error closing file: %s", ITEMS_CONFIG_FILEPATH);
    }
}



/* Para cualquier clase que no sea esperada, nos quejamos */
template <class T>
void Config<T>::_parseFile() {
    throw Exception("Unknown configuration requested.");
}

//-----------------------------------------------------------------------------
