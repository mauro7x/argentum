#include "../includes/Config.h"

//-----------------------------------------------------------------------------
// Métodos privados

template <class T>
json Config<T>::_loadJsonFile(std::string filepath) const {
    std::ifstream file(filepath);
    if (file.fail()) {
        throw Exception("Error opening file: %s", filepath);
    }

    json j;
    file >> j;
    if (file.fail()) {
        throw Exception("Error reading file: %s", filepath);
    }

    file.close();
    if (file.fail()) {
        throw Exception("Error closing file: %s", filepath);
    }

    return j;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

template <>
void Config<RaceCfg>::_parseFile() {
    json j = _loadJsonFile(RACE_CONFIG_FILEPATH);

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
}

template <>
void Config<KindCfg>::_parseFile() {
    json j = _loadJsonFile(KIND_CONFIG_FILEPATH);

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
}

template <>
void Config<NPCCfg>::_parseFile() {
    json j = _loadJsonFile(NPC_CONFIG_FILEPATH);

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
    json j = _loadJsonFile(ITEMS_CONFIG_FILEPATH);

    int size = j["weapons"].size();
    for (int i = 0; i < size; i++) {
        WeaponCfg weapon;

        weapon.id = j["weapons"][i]["id"];
        weapon.name = j["weapons"][i]["name"];
        weapon.type = j["weapons"][i]["type"];
        weapon.price = j["weapons"][i]["price"];
        weapon.min_damage = j["weapons"][i]["min_damage"];
        weapon.max_damage = j["weapons"][i]["max_damage"];
        weapon.distant_attack = j["weapons"][i]["distant_attack"];

        config[weapon.id] = weapon;
    }
}

template <>
void Config<WandCfg>::_parseFile() {
    json j = _loadJsonFile(ITEMS_CONFIG_FILEPATH);

    int size = j["wands"].size();
    for (int i = 0; i < size; i++) {
        WandCfg wand;

        wand.id = j["wands"][i]["id"];
        wand.name = j["wands"][i]["name"];
        wand.type = j["wands"][i]["type"];
        wand.price = j["wands"][i]["price"];
        wand.min_damage = j["wands"][i]["min_damage"];
        wand.max_damage = j["wands"][i]["max_damage"];
        wand.mana_usage_cost = j["wands"][i]["mana_usage_cost"];

        config[wand.id] = wand;
    }
}

template <>
void Config<DefenceCfg>::_parseFile() {
    json j = _loadJsonFile(ITEMS_CONFIG_FILEPATH);

    int size = j["defences"].size();
    for (int i = 0; i < size; i++) {
        DefenceCfg defence;

        defence.id = j["defences"][i]["id"];
        defence.name = j["defences"][i]["name"];
        defence.price = j["defences"][i]["price"];
        defence.min_defence = j["defences"][i]["min_defence"];
        defence.max_defence = j["defences"][i]["max_defence"];

        config[defence.id] = defence;
    }
}

/* Para cualquier clase que no sea esperada, nos quejamos */
template <class T>
void Config<T>::_parseFile() {
    throw Exception("Unknown configuration requested.");
}

//-----------------------------------------------------------------------------
