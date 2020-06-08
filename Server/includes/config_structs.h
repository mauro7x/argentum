#ifndef __CONFIG_STRUCTS_H__
#define __CONFIG_STRUCTS_H__

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifndef __ID__
#define __ID__
typedef int Id;
#endif  // __ID__
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifndef __WEARABLE_TYPE_H__
#define __WEARABLE_TYPE_H__
enum WearableType { HELMET, ARMOUR, SHIELD, WEAPON, N_WEARABLE_ITEMS };
#endif
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Objetos

struct WeaponCfg {
    int id;
    std::string name;
    WearableType type;
    unsigned int price;
    unsigned int min_damage;
    unsigned int max_damage;
    bool distant_attack;
};

struct WandCfg {
    int id;
    std::string name;
    WearableType type;
    unsigned int price;
    unsigned int min_damage;
    unsigned int max_damage;
    unsigned int mana_usage_cost;
};

struct DefenceCfg {
    int id;
    std::string name;
    WearableType type;
    int price;
    int min_defence;
    int max_defence;
};

struct PotionCfg {
    unsigned int id;
    std::string name;
    unsigned int price;
    unsigned int recovery_points;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Razas

struct RaceCfg {
    int id;
    std::string name;
    int health;
    int recovery;
    int mana;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Clases

struct KindCfg {
    int id;
    std::string name;
    int health;
    int meditation;
    int mana;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// NPCs

struct NPCCfg {
    int id;
    std::string name;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#endif  // __CONFIG_STRUCTS_H__
