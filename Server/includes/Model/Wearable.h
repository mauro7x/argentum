#ifndef __WEARABLE_H__
#define __WEARABLE_H__

#include <string>

#include "../../../Common/includes/types.h"
#include "Item.h"
#include "config_structs.h"

/*
 * Define la abstraccion Wearable.
 * Un Wearable es un Item que puede ser portado en
 * Equipment, y tiene un uso específico sobre otro jugador
 * (o sobre sí mismo), que puede
 * ser tanto de ataque, defensa o curación.
 */
class Wearable : public Item {
   protected:
    WearableType type;

   public:
    Wearable(const Id id, const std::string name, const unsigned int price,
             WearableType type);
    virtual ~Wearable();

    /*
     * Double dispatch. Hace que el jugador se equipe
     * del wearable.
     */
    virtual void equip(Character& equipper);

    /*
     * Devuelve los puntos de daño, curación o defensa
     * según el tipo de wearable.
     */
    virtual const unsigned int use(Character& user) = 0;

    /* Si es de ataque, devuelve el rango del wearable */
    virtual const unsigned int getRange() const = 0;

    /* Si es de lanzamiento, devuelve si el wearable es curativo */
    virtual const bool isHealing() const = 0;

    /* Devuelve el tipo de wearable */
    const WearableType getType() const;
};

#endif
