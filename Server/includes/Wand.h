#ifndef __WAND_H__
#define __WAND_H__

#include "Wearable.h"

class Wand: public Wearable {
    private:
        const unsigned int mana_usage_cost;
        
    public:
         Wand(const unsigned int id, const unsigned int price,
              WearableType type,
              const unsigned int min_damage, 
              const unsigned int max_damage,
              const unsigned int mana_usage_cost);
        ~Wand();

        Wand(const Wand&) = delete;
        Wand& operator=(const Wand&) = delete;
        Wand(Wand&&) = delete;
        Wand& operator=(Wand&&) = delete;

        // IMPLEMENTAR Metodos comodamente en base
        // a los metodos definidos del Character/Inventory/Equipment.
        // Mantener abstraccion item y Wearable.

        virtual void equip(Character& character) override;
        virtual const unsigned int use() override;
};

#endif
