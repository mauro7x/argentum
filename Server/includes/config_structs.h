#ifndef __CONFIG_STRUCTS_H__
#define __CONFIG_STRUCTS_H__

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifndef __ID__
#define __ID__
typedef int Id;
#endif __ID__
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Objetos

enum WeaponType { MELEE = 0, RANGED, MAGIC };
struct WeaponCfg {
    int id;
    std::string name;
    WeaponType type;
    int min_dmg;
    int max_dmg;
    int price;
};

struct DefenceItem {
    int id;
    std::string name;
    int min_def;
    int max_def;
    int price;
};

struct HelmetCfg {
    int id;
    std::string name;
    int min_def;
    int max_def;
    int price;
};

struct ArmourCfg {
    int id;
    std::string name;
    int min_def;
    int max_def;
    int price;
};

struct ShieldCfg {
    int id;
    std::string name;
    int min_def;
    int max_def;
    int price;
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
