#ifndef __WAND_H__
#define __WAND_H__

#include "Item.h"

class Wand: public Item {
    private:
        const bool distant_attack;
        const unsigned int min_damage, max_damage;
        const unsigned int mana_usage_cost;
        
    public:
         Wand(const unsigned int id, const unsigned int buy_price, 
              const unsigned int sell_price, const bool distant_attack, 
              const unsigned int min_damage, const unsigned int max_damage,
              const unsigned int mana_usage_cost);
        ~Wand();

        Wand(const Wand&) = delete;
        Wand& operator=(const Wand&) = delete;
        Wand(Wand&&) = delete;
        Wand& operator=(Wand&&) = delete;

        // IMPLEMENTAR Metodos comodamente en base
        // a los metodos definidos del Character/Inventory/Equipment.
        // Mantener abstraccion item.

        virtual void use(Character& character) override;
        
        const unsigned int getMinDamagePoints() const;
        const unsigned int getMaxDamagePoints() const;
};

#endif
