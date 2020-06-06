#ifndef __WEAPON_H__
#define __WEAPON_H__

#include <string>
#include "Item.h"

class Weapon: public Item {
    private:
        const bool distant_attack;
        const unsigned int min_damage, max_damage;

    public:
        Weapon(const unsigned int id, const unsigned int buy_price, 
               const unsigned int sell_price, const bool distant_attack, 
               const unsigned int min_damage, const unsigned int max_damage);
        ~Weapon();

        Weapon(const Weapon&) = delete;
        Weapon& operator=(const Weapon&) = delete;
        Weapon(Weapon&&) = delete;
        Weapon& operator=(Weapon&&) = delete;

        // IMPLEMENTAR Metodos comodamente en base
        // a los metodos definidos del Character/Inventory/Equipment.
        // Mantener abstraccion Item.

        virtual void use(Character& character) override;

        const unsigned int getMinDamagePoints() const;
        const unsigned int getMaxDamagePoints() const;
};

#endif