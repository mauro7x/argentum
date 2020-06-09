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
        weapon.type = WEAPON;
        weapon.price = j["weapons"][i]["price"];
        weapon.min_damage = j["weapons"][i]["min_damage"];
        weapon.max_damage = j["weapons"][i]["max_damage"];
        weapon.attack_distance = j["weapons"][i]["attack_distance"];
        weapon.mana_usage_cost = j["weapons"][i]["mana_usage_cost"];

        config[weapon.id] = weapon;
    }
}

template <>
void Config<DefenceCfg>::_parseFile() {
    json j = _loadJsonFile(ITEMS_CONFIG_FILEPATH);

    /* Cascos */
    int size = j["helmets"].size();
    for (int i = 0; i < size; i++) {
        DefenceCfg helmet;

        helmet.id = j["helmets"][i]["id"];
        helmet.name = j["helmets"][i]["name"];
        helmet.type = HELMET;
        helmet.price = j["helmets"][i]["price"];
        helmet.min_defence = j["helmets"][i]["min_defence"];
        helmet.max_defence = j["helmets"][i]["max_defence"];

        config[helmet.id] = helmet;
    }

    /* Armaduras */
    size = j["armours"].size();
    for (int i = 0; i < size; i++) {
        DefenceCfg armour;

        armour.id = j["armours"][i]["id"];
        armour.name = j["armours"][i]["name"];
        armour.type = ARMOUR;
        armour.price = j["armours"][i]["price"];
        armour.min_defence = j["armours"][i]["min_defence"];
        armour.max_defence = j["armours"][i]["max_defence"];

        config[armour.id] = armour;
    }

    /* Escudos */
    size = j["shields"].size();
    for (int i = 0; i < size; i++) {
        DefenceCfg shield;

        shield.id = j["shields"][i]["id"];
        shield.name = j["shields"][i]["name"];
        shield.type = SHIELD;
        shield.price = j["shields"][i]["price"];
        shield.min_defence = j["shields"][i]["min_defence"];
        shield.max_defence = j["shields"][i]["max_defence"];

        config[shield.id] = shield;
    }
}

/* Para cualquier clase que no sea esperada, nos quejamos */
template <class T>
void Config<T>::_parseFile() {
    throw Exception("Unknown configuration requested.");
}

//-----------------------------------------------------------------------------
