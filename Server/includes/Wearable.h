#ifndef __WEARABLE_H__
#define __WEARABLE_H__

#include <string>

#include "Item.h"
#include "config_structs.h"

/*
 * Define la abstraccion Wearable.
 * Un Wearable es un Item que tiene puntos de uso
 * minimos y maximos, con un determinado fin (ataque/defensa)
 * y se puede portar en un Equipment.
 */
class Wearable: public Item {
    protected:
        WearableType type;

    public:
        Wearable(const unsigned int id, const std::string name,
                 const unsigned int price,
                 WearableType type);
        virtual ~Wearable();
        // Poner las cosas que tienen en comun armas y defensas que van en Equipment.

        virtual void equip(Character& equipper);
        virtual const unsigned int use(Character& user) = 0;
        
        const WearableType getType() const;
        virtual const unsigned int getRange() const;
};

#endif
