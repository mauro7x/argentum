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
        weapon.type = j["weapons"][i]["type"];
        weapon.min_damage = j["weapons"][i]["min_damage"];
        weapon.max_damage = j["weapons"][i]["max_damage"];
        weapon.price = j["weapons"][i]["price"];
        weapon.distant_attack = j["weapons"][i]["distant_attack"];

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
        wand.type = j["wands"][i]["type"];
        wand.min_damage = j["wands"][i]["min_damage"];
        wand.max_damage = j["wands"][i]["max_damage"];
        wand.price = j["wands"][i]["price"];
        wand.mana_usage_cost = j["wands"][i]["mana_usage_cost"];

        config[wand.id] = wand;
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

    int size = j["defences"].size();
    for (int i = 0; i < size; i++) {
        DefenceCfg defence;

        defence.id = j["defences"][i]["id"];
        defence.name = j["defences"][i]["name"];
        defence.min_defence = j["defences"][i]["min_defence"];
        defence.max_defence = j["defences"][i]["max_defence"];
        defence.price = j["defences"][i]["price"];

        config[defence.id] = defence;
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
