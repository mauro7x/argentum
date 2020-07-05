#ifndef __DEFENCE_H__
#define __DEFENCE_H__
//-----------------------------------------------------------------------------
#include "Wearable.h"
#include "config_structs.h"
//-----------------------------------------------------------------------------

/*
 * Representa las distintas defensas que un character puede portar.
 * i.e: cascos, armaduras, y escudos.
 *
 * Cada cual tiene sus puntos de defensa, que restan puntos de daño ante un
 * ataque externo.
 */
class Defence : public Wearable {
   private:
    unsigned int min_defence, max_defence;

   public:
    Defence(const DefenceCfg& data);
    ~Defence();

    Defence(const Defence&) = delete;
    Defence& operator=(const Defence&) = delete;
    Defence(Defence&&) = delete;
    Defence& operator=(Defence&&) = delete;

    /* Devuelve los puntos de defensa que absorbe. */
    const unsigned int use(Character& user) override;

    /* Lanza excepción. La defensa no tiene rango */
    const unsigned int getRange() const override;

    /*
     * Lanza excepción. La no es un arma.
     */
    const WeaponType getWeaponType() const override;
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
