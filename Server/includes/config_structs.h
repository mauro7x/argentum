#ifndef __CONFIG_STRUCTS_H__
#define __CONFIG_STRUCTS_H__

//-----------------------------------------------------------------------------
#include <cstdint>
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
    Id id;
    std::string name;
    WearableType type;
    uint16_t price;
    uint16_t min_damage;
    uint16_t max_damage;
    uint8_t attack_distance;
    uint8_t mana_usage_cost;
};

struct DefenceCfg {
    Id id;
    std::string name;
    WearableType type;
    uint16_t price;
    uint16_t min_defence;
    uint16_t max_defence;
};

struct PotionCfg {
    Id id;
    std::string name;
    uint16_t price;
    uint16_t recovery_points;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Razas

struct RaceCfg {
    Id id;
    std::string name;
    uint8_t health;
    uint8_t recovery;
    uint8_t mana;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Clases

struct KindCfg {
    Id id;
    std::string name;
    uint8_t health;
    uint8_t meditation;
    uint8_t mana;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// NPCs

struct NPCCfg {
    Id id;
    std::string name;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#endif  // __CONFIG_STRUCTS_H__
