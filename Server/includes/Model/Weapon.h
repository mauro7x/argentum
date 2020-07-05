#ifndef __WEAPON_H__
#define __WEAPON_H__
//-----------------------------------------------------------------------------
#include "Wearable.h"
#include "config_structs.h"
//-----------------------------------------------------------------------------
class Weapon: public Wearable {
    private:
        const unsigned int range;
        const unsigned int min_damage;
        const unsigned int max_damage;
        const unsigned int cooldown;
        const WeaponType weapon_type;

    public:
        Weapon(const WeaponCfg& data);
        ~Weapon();

        Weapon(const Weapon&) = delete;
        Weapon& operator=(const Weapon&) = delete;
        Weapon(Weapon&&) = delete;
        Weapon& operator=(Weapon&&) = delete;
        
        /*
         * Setea el cooldown de uso de arma en el character
         * y retorna los puntos de daño que causa.
         */
        const unsigned int use(Character& user) override;

        /* Devuelve el rango del arma */
        const unsigned int getRange() const override;

        /* Devuelve si es curativa [always false] */
        const WeaponType getWeaponType() const override;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
