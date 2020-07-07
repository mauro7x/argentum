#ifndef __CONFIG_STRUCTS_H__
#define __CONFIG_STRUCTS_H__
//-----------------------------------------------------------------------------
#include <cstdint>
#include <list>
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Inventory.h"
#include "../../../Common/includes/Orientation.h"
#include "../../../Common/includes/defs.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifndef __WEAPON_TYPE_H__
#define __WEAPON_TYPE_H__
enum WeaponType { MELEE_ATTACK, DISTANT_ATTACK, EXPLOSIVE, HEALING };
#endif
//-----------------------------------------------------------------------------
#ifndef __POTION_TYPE_H__
#define __POTION_TYPE_H__
enum PotionType { HEALTH, MANA };
#endif
//-----------------------------------------------------------------------------

#define AMOUNT_OF_DEFFENCE_TYPES 3

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
    WeaponType weapon_type;
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
    unsigned int mana_usage_cost;
    unsigned int cooldown;
    WeaponType weapon_type;

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

    // Modificadores
    int intelligence;
    int constitution;
    int strength;
    int agility;

    // Verificar si es enano para el renderizado
    bool is_shorter;
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
    std::list<Id> sellable_items;
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
    char nickname[MAX_USERNAME_SIZE + 1] = {0};

    Id race = 0;
    Id kind = 0;
    Id head_id = 0;
    Id body_id = 0;

    StateType state = ALIVE;

    EquipmentData equipment = {0};
    InventoryData inventory = {0};
    unsigned int bank_gold = 0;
    BankAccountData bank_account = {0};

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
    unsigned int min_level = 0;
    unsigned int max_level = 0;
    unsigned int min_damage = 0;
    unsigned int max_damage = 0;
    unsigned int base_health = 0;
    unsigned int visible_range = 0;
    unsigned int movement_speed = 0;
    unsigned int attack_cooldown = 0;
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
