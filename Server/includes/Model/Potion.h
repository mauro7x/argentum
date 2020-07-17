#ifndef __POTION_H__
#define __POTION_H__
//-----------------------------------------------------------------------------
#include <exception>
#include <string>
//-----------------------------------------------------------------------------
#include "Item.h"
#include "config_structs.h"
//-----------------------------------------------------------------------------
class Character;  // Forward declaration p/ evitar circular dependencies
//-----------------------------------------------------------------------------

class Potion : public Item {
   protected:
    const unsigned int recovery_points;

   public:
    Potion(const Id id, const std::string name, const unsigned int price,
           const unsigned int recovery_points);
    virtual ~Potion();

    Potion(const Potion&) = delete;
    Potion& operator=(const Potion&) = delete;
    Potion(Potion&&) = delete;
    Potion& operator=(Potion&&) = delete;

    virtual Response equip(Character& equipper) = 0;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class PotionFactory {
   public:
    static Potion* newPotion(const PotionCfg& data);
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class HealthPotion : public Potion {
   public:
    HealthPotion(const PotionCfg& data);
    ~HealthPotion();

    virtual Response equip(Character& equipper) override;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ManaPotion : public Potion {
   public:
    ManaPotion(const PotionCfg& data);
    ~ManaPotion();

    virtual Response equip(Character& equipper) override;
};

//-----------------------------------------------------------------------------

class UnknownPotionTypeException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
