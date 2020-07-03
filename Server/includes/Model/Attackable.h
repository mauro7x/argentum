#ifndef __ATTACKABLE_H__
#define __ATTACKABLE_H__
//-----------------------------------------------------------------------------
#include <vector>
//-----------------------------------------------------------------------------
#include "Position.h"
#include "config_structs.h"
//-----------------------------------------------------------------------------

class Attackable {
   public:
    virtual const bool receiveAttack(int& damage, const bool eludible) = 0;

    virtual void dropAllItems(std::vector<DroppingSlot>& dropped_items) = 0;

    virtual const Position& getPosition() const = 0;

    virtual const unsigned int getLevel() const = 0;

    virtual const unsigned int getHealth() const = 0;

    virtual const unsigned int getMaxHealth() const = 0;

    virtual const Id getMapId() const = 0;

    virtual const bool recoverHealth(unsigned int& points) = 0;

    virtual void beAttacked() = 0;

    virtual const bool isNewbie() const = 0;

    virtual const bool isCreature() const = 0;
};

//-----------------------------------------------------------------------------
#endif  // __ATTACKABLE_H__