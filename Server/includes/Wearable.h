#ifndef __WEARABLE_H__
#define __WEARABLE_H__

#include "Character.h"
enum WearableType {HELMET, ARMOUR, SHIELD, WEAPON, N_WEARABLE_ITEMS};

/*
 * Define la abstraccion Wearable.
 * Un Wearable es un Item que tiene puntos de uso
 * minimos y maximos, con un determinado fin (ataque/defensa)
 * y se puede portar en un Equipment.
 */
class Wearable: public Item {
    protected:
        WearableType type;
        const unsigned int min_usage_points;
        const unsigned int max_usage_points;
        const unsigned int getUsagePoints();

    public:
        Wearable(const unsigned int id, const unsigned int price,
                 WearableType type,
                 const unsigned int min_usage_points,
                 const unsigned int max_usage_points);

        // Poner las cosas que tienen en comun armas y defensas que van en Equipment.

        virtual void equip(Character& character);
        virtual const unsigned int use() = 0;
        
        const WearableType getType() const;
};

#endif
