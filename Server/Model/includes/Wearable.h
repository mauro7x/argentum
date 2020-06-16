#ifndef __WEARABLE_H__
#define __WEARABLE_H__

#include <string>

#include "Item.h"
#include "config_structs.h"

#include "../../../Common/includes/types.h"

/*
 * Define la abstraccion Wearable.
 * Un Wearable es un Item que puede ser portado en
 * Equipment, y tiene un uso específico, que puede
 * ser tanto de ataque, defensa o curación.
 */
class Wearable: public Item {
    protected:
        WearableType type;

    public:
        Wearable(const Id id, const std::string name,
                 const unsigned int price,
                 WearableType type);
        virtual ~Wearable();

        virtual void equip(Character& equipper);
        
        virtual const unsigned int use(Character& user) = 0;
        
        const WearableType getType() const;
        virtual const unsigned int getRange() const;
};

#endif
