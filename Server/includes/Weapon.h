#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Wearable.h"

class Weapon: public Wearable {
    private:
        const bool distant_attack;

    public:
        Weapon(const unsigned int id, const unsigned int price, 
               const unsigned int min_damage, 
               const unsigned int max_damage,
               const bool distant_attack);
        ~Weapon();

        Weapon(const Weapon&) = delete;
        Weapon& operator=(const Weapon&) = delete;
        Weapon(Weapon&&) = delete;
        Weapon& operator=(Weapon&&) = delete;

        // IMPLEMENTAR Metodos comodamente en base
        // a los metodos definidos del Character/Inventory/Equipment.
        // Mantener abstraccion Item y Wearable.
        
        virtual void equip(Character& character) override;
        virtual const unsigned int use() override;
};

#endif
