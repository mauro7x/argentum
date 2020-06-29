#ifndef __CONFIG_STRUCTS_H__
#define __CONFIG_STRUCTS_H__

//-----------------------------------------------------------------------------
#include <cstdint>
#include <string>
#include <vector>

#include "../../../Common/includes/Inventory.h"
#include "../../../Common/includes/Orientation.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifndef __SPELL_TYPE_H__
#define __SPELL_TYPE_H__
enum SpellType { ATTACKING, HEALING };
#endif
//-----------------------------------------------------------------------------
#ifndef __POTION_TYPE_H__
#define __POTION_TYPE_H__
enum PotionType { HEALTH, MANA };
#endif
//-----------------------------------------------------------------------------

#define AMOUNT_OF_DEFFENCE_TYPES 3
#define NICKNAME_MAX_LENGTH 8

//-----------------------------------------------------------------------------
// Objetos

struct WeaponCfg {
    Id id;
    std::string name;
    unsigned int price;
    unsigned int range;
    unsigned int min_damage;
    unsigned int max_damage;
    unsigned int cooldown;
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
    unsigned int cooldown;

    // ATTACK SPELL ONLY
    unsigned int range;
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

struct OtherItemsCfg {
    Id id;
    std::string name;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Razas

struct RaceCfg {
    Id id;
    std::string name;
    Id dead_head_id;
    Id dead_body_id;
    std::vector<Id> head_ids;
    std::vector<Id> body_ids;

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
enum StateType { DEAD, ALIVE };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
struct CharacterCfg {
    Id map = 0;
    int x_tile = 0, y_tile = 0;
    char nickname[NICKNAME_MAX_LENGTH] = {0};

    Id race = 0;
    Id kind = 0;
    Id head_id = 0;
    Id body_id = 0;

    StateType state = ALIVE;

    EquipmentData equipment = {0};
    InventoryData inventory = {0};

    uint32_t health = 0;
    uint32_t mana = 0;
    uint32_t safe_gold = 0;
    uint32_t excess_gold = 0;
    uint32_t level = 0;
    uint32_t exp = 0;
    bool new_created = true;
};

struct CreatureCfg {
    Id id = 0;
    std::string name;
    unsigned int base_damage = 0;
    unsigned int base_health = 0;
};
//-----------------------------------------------------------------------------
struct DroppingSlot {
    Id item;
    uint32_t amount;

    DroppingSlot(Id item, uint32_t amount) : item(item), amount(amount) {}
    ~DroppingSlot() {}
};
//-----------------------------------------------------------------------------

#endif  // __CONFIG_STRUCTS_H__
