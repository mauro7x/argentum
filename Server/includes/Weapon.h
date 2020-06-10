#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Wearable.h"
#include "config_structs.h"

class Weapon: public Wearable {
    private:
        const unsigned int attack_distance;
        const unsigned int min_damage;
        const unsigned int max_damage;

    public:
        Weapon(const WeaponCfg& data);
        ~Weapon();

        Weapon(const Weapon&) = delete;
        Weapon& operator=(const Weapon&) = delete;
        Weapon(Weapon&&) = delete;
        Weapon& operator=(Weapon&&) = delete;

        // IMPLEMENTAR Metodos comodamente en base
        // a los metodos definidos del Character/Inventory/Equipment.
        // Mantener abstraccion Item y Wearable.
        
        virtual const unsigned int use(Character& user) override;
};

#endif
