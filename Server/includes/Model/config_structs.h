#ifndef __CONFIG_STRUCTS_H__
#define __CONFIG_STRUCTS_H__

//-----------------------------------------------------------------------------
#include <cstdint>
#include <string>

#include "../../../Common/includes/types.h"
#include "../../../Common/includes/Inventory.h"
#include "../../../Common/includes/Orientation.h"
//-----------------------------------------------------------------------------

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
    Id id;
    std::string name;
    unsigned int price;
    unsigned int attack_distance;
    unsigned int min_damage;
    unsigned int max_damage;
};

struct WandCfg {
    Id id;
    std::string name;
    Id spell_id;
    unsigned int price;
};

struct SpellCfg {
    Id id;
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

    Id head_id;
    Id body_id;

    uint8_t max_health_factor;
    uint8_t max_mana_factor;
    uint8_t health_recovery_factor;
    uint8_t mana_recovery_factor;
    
    // Atributos iniciales
    uint8_t initial_mana;
    uint8_t initial_health;
    
    // Modificadores
    int intelligence;
    int constitution;
    int strength;
    int agility;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Clases

struct KindCfg {
    Id id;
    std::string name;
    uint8_t max_health_factor;
    uint8_t max_mana_factor;
    uint8_t meditation_factor;

    // Atributos iniciales
    uint8_t initial_mana;
    uint8_t initial_health;

    // Modificadores
    int intelligence;
    int constitution;
    int strength;
    int agility;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// NPCs

struct NPCCfg {
    Id id;
    std::string name;
};

//-----------------------------------------------------------------------------
// PERSISTENCIA
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
enum StateType {DEAD, ALIVE};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
struct CharacterCfg {
    Id race;
    Id kind;

    StateType state;

    EquipmentData equipment;
    InventoryData inventory;

    uint32_t health;
    uint32_t mana;
    uint32_t safe_gold;
    uint32_t excess_gold;
    uint32_t level;
    uint32_t exp;
};
//-----------------------------------------------------------------------------
#endif  // __CONFIG_STRUCTS_H__
