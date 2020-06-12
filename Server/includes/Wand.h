#ifndef __WAND_H__
#define __WAND_H__

#include "Wearable.h"
#include "Spell.h"
#include "config_structs.h"

class Wand: public Wearable {
    private:
        Spell* spell;
        
    public:
         Wand(const WandCfg& wand_data, 
              const SpellCfg& spell_data);
        ~Wand();

        Wand(const Wand&) = delete;
        Wand& operator=(const Wand&) = delete;
        Wand(Wand&&) = delete;
        Wand& operator=(Wand&&) = delete;

        // IMPLEMENTAR Metodos comodamente en base
        // a los metodos definidos del Character/Inventory/Equipment.
        // Mantener abstraccion item y Wearable.

        virtual const unsigned int use(Character& user) override;
        const unsigned int getRange() const override;
};

#endif
