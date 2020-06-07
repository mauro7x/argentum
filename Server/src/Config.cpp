#include "../includes/Config.h"

//-----------------------------------------------------------------------------
// API Pública

template <>
void Config<RaceCfg>::_parseFile() {
    std::ifstream file(RACE_CONFIG_FILEPATH);
    json j;
    file >> j;

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
}

template <>
void Config<KindCfg>::_parseFile() {
    std::ifstream file(KIND_CONFIG_FILEPATH);
    json j;
    file >> j;

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
}

template <>
void Config<NPCCfg>::_parseFile() {
    std::ifstream file(NPC_CONFIG_FILEPATH);
    json j;
    file >> j;

    int size = j.size();
    for (int i = 0; i < size; i++) {
        NPCCfg npc;

        npc.id = j[i]["id"];
        npc.name = j[i]["name"];
        config[npc.id] = npc;
    }

    file.close();
}

template <>
void Config<WeaponCfg>::_parseFile() {
    std::ifstream file(ITEMS_CONFIG_FILEPATH);
    json j;
    file >> j;

    int size = j["weapons"].size();
    for (int i = 0; i < size; i++) {
        WeaponCfg weapon;

        weapon.id = j["weapons"][i]["id"];
        weapon.name = j["weapons"][i]["name"];
        weapon.type = j["weapons"][i]["type"];
        weapon.min_dmg = j["weapons"][i]["min_dmg"];
        weapon.max_dmg = j["weapons"][i]["max_dmg"];
        weapon.price = j["weapons"][i]["price"];

        config[weapon.id] = weapon;
    }

    file.close();
}

template <>
void Config<HelmetCfg>::_parseFile() {
    std::ifstream file(ITEMS_CONFIG_FILEPATH);
    json j;
    file >> j;

    int size = j["helmets"].size();
    for (int i = 0; i < size; i++) {
        HelmetCfg helmet;

        helmet.id = j["helmets"][i]["id"];
        helmet.name = j["helmets"][i]["name"];
        helmet.min_def = j["helmets"][i]["min_def"];
        helmet.max_def = j["helmets"][i]["max_def"];
        helmet.price = j["helmets"][i]["price"];

        config[helmet.id] = helmet;
    }

    file.close();
}

template <>
void Config<ArmourCfg>::_parseFile() {
    std::ifstream file(ITEMS_CONFIG_FILEPATH);
    json j;
    file >> j;

    int size = j["armours"].size();
    for (int i = 0; i < size; i++) {
        ArmourCfg armour;

        armour.id = j["armours"][i]["id"];
        armour.name = j["armours"][i]["name"];
        armour.min_def = j["armours"][i]["min_def"];
        armour.max_def = j["armours"][i]["max_def"];
        armour.price = j["armours"][i]["price"];

        config[armour.id] = armour;
    }

    file.close();
}

template <>
void Config<ShieldCfg>::_parseFile() {
    std::ifstream file(ITEMS_CONFIG_FILEPATH);
    json j;
    file >> j;

    int size = j["shields"].size();
    for (int i = 0; i < size; i++) {
        ShieldCfg shield;

        shield.id = j["shields"][i]["id"];
        shield.name = j["shields"][i]["name"];
        shield.min_def = j["shields"][i]["min_def"];
        shield.max_def = j["shields"][i]["max_def"];
        shield.price = j["shields"][i]["price"];

        config[shield.id] = shield;
    }

    file.close();
}

/* Para cualquier clase que no sea esperada, nos quejamos */
template <class T>
void Config<T>::_parseFile() {
    throw Exception("Unknown configuration requested.");
}

//-----------------------------------------------------------------------------
