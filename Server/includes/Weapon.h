#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Wearable.h"
#include "config_structs.h"

class Weapon: public Wearable {
    private:
        const bool distant_attack;

    public:
        Weapon(WeaponCfg& data);
        ~Weapon();

        Weapon(const Weapon&) = delete;
        Weapon& operator=(const Weapon&) = delete;
        Weapon(Weapon&&) = delete;
        Weapon& operator=(Weapon&&) = delete;

        // IMPLEMENTAR Metodos comodamente en base
        // a los metodos definidos del Character/Inventory/Equipment.
        // Mantener abstraccion Item y Wearable.
        
        virtual const unsigned int use() override;
};

#endif
