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
#ifndef __SPELL_TYPE_H__
#define __SPELL_TYPE_H__
enum SpellType {ATTACKING, HEALING};
#endif
//-----------------------------------------------------------------------------
#ifndef __POTION_TYPE_H__
#define __POTION_TYPE_H__
enum PotionType {HEALTH, MANA};
#endif
//-----------------------------------------------------------------------------

#define AMOUNT_OF_DEFFENCE_TYPES 3

//-----------------------------------------------------------------------------
// Objetos

struct WeaponCfg {
    int id;
    std::string name;
    unsigned int price;
    unsigned int attack_distance;
    unsigned int min_damage;
    unsigned int max_damage;
};

struct WandCfg {
    int id;
    std::string name;
    int spell_id;
    unsigned int price;
};

struct SpellCfg {
    int id;
    std::string name;
    SpellType type;
    unsigned int mana_usage_cost;

    // ATTACK SPELL ONLY
    unsigned int attack_distance;
    unsigned int min_damage;
    unsigned int max_damage;

    // HEALING SPELL ONLY
    unsigned int recovery_points;
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
    PotionType type;
    unsigned int price;
    unsigned int recovery_points;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Razas

struct RaceCfg {
    Id id;
    std::string name;
    uint8_t max_health_factor;
    uint8_t health_recovery_factor;
    uint8_t max_mana_factor;
    // Modificadores
    float intelligence;
    float constitution;
    float strength;
    float agility;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Clases

struct KindCfg {
    Id id;
    std::string name;
    uint8_t max_health_factor;
    uint8_t meditation_factor;
    uint8_t max_mana_factor;
    uint8_t initial_mana;
    uint8_t initial_health;
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
