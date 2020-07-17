#ifndef __WEARABLE_H__
#define __WEARABLE_H__
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------
#include "Item.h"
#include "Response.h"
#include "config_structs.h"
//-----------------------------------------------------------------------------
/*
 * Define la abstraccion Wearable.
 * Un Wearable es un Item que puede ser portado en
 * Equipment, y tiene un uso específico sobre otro jugador
 * (o sobre sí mismo), que puede
 * ser tanto de ataque, defensa o curación.
 */
class Wearable : public Item {
   protected:
    WearableType wearable_type;

   public:
    Wearable(const Id id, const std::string name, const unsigned int price,
             WearableType wearable_type);
    virtual ~Wearable();

    /*
     * Double dispatch. Hace que el jugador se equipe
     * del wearable.
     */
    virtual Response equip(Character& equipper);

    /*
     * Devuelve los puntos de daño, curación o defensa
     * según el tipo de wearable.
     */
    virtual Response use(Character& user, int& points) = 0;

    /* Si es de ataque, devuelve el rango del wearable. */
    virtual const unsigned int getRange() const = 0;

    /* (Para las armas de ataque únicamente) Devuelve el tipo de weapon. */
    virtual const WeaponType getWeaponType() const = 0;

    /* Devuelve el tipo de wearable. */
    const WearableType getType() const;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
