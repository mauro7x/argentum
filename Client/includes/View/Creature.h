#ifndef __CREATURE_H__
#define __CREATURE_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/types.h"
#include "Unit.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Creature : public Unit {
   private:
    // Cuerpo
    Id creature_id;

    // Vida
    uint32_t health;

    /* Copia la data desde el paquete recibido */
    void _copyData(const CreatureData& init_data);

   public:
    /* Constructor */
    Creature(Renderer* renderer, UnitSpriteContainer* sprites, const int tile_w,
             const int tile_h, const float tile_movement_time);

    /* Deshabilitamos el constructor por copia. */
    Creature(const Creature&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Creature& operator=(const Creature&) = delete;

    /* Habilitamos el constructor por movimiento. */
    Creature(Creature&& other);

    /* Habilitamos el operador= para movimiento. */
    Creature& operator=(Creature&& other);

    //-------------------------------------------------------------------------

    /* Se inicializa con el primer paquete del server */
    void init(const CreatureData& init_data);

    /* Actualizar información según lo que diga el servidor */
    void update(const CreatureData& updated_data);

    /* Renderizarse si se encuentra dentro de la cámara */
    void render() const override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Creature();
};

//-----------------------------------------------------------------------------

#endif  // __CREATURE_H__
