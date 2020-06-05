#ifndef __WEAPON_H__
#define __WEAPON_H__

#include <string>
#include "Item.h"

class Weapon: public Item {
    private:
        const std::string name;
        const bool distant_attack;
        const unsigned int damage, buy_price, sell_price;

    public:
        Weapon(const std::string name, const bool distant_attack, 
               const unsigned int damage, const unsigned int buy_price,
               const unsigned int sell_price);
        ~Weapon();

        Weapon(const Weapon&) = delete;
        Weapon& operator=(const Weapon&) = delete;
        Weapon(Weapon&&) = delete;
        Weapon& operator=(Weapon&&) = delete;

        // IMPLEMENTAR Metodos comodamente en base
        // a los metodos definidos del Character/Inventory/Equipment.
        // Mantener abstraccion Item.

};

#endif