#ifndef __WAND_H__
#define __WAND_H__

#include <string>
#include "Item.h"

class Wand: public Item {
    private:
        const std::string name;
        const bool distant_attack;
        const unsigned int damage, buy_price, sell_price, manna_usage_cost;
        
    public:
        Wand(const std::string name, const bool distant_attack,
             const unsigned int damage, const unsigned int buy_price, 
             const unsigned int sell_price,
             const unsigned int manna_usage_cost);
        ~Wand();

        Wand(const Wand&) = delete;
        Wand& operator=(const Wand&) = delete;
        Wand(Wand&&) = delete;
        Wand& operator=(Wand&&) = delete;

        // IMPLEMENTAR Metodos comodamente en base
        // a los metodos definidos del Character/Inventory/Equipment.
        // Mantener abstraccion item.
};

#endif