#ifndef __CREATURE_H__
#define __CREATURE_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/defs.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Unit.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Creature : public Unit {
   private:
    // Datos del monstruo
    Id creature_id = 0;
    uint32_t health = 0;

    // Texturas para el nickname
    const TTF_Font* g_nickname_font; /* no se debe cerrar */
    const TTF_Font* g_level_font;    /* no se debe cerrar */
    Texture info_nickname;
    Texture info_level;

    /* Copia la data desde el paquete recibido */
    void _copyData(const CreatureData& init_data);

   public:
    /* Constructor */
    Creature(Renderer* renderer, UnitSpriteContainer* sprites,
             const TTF_Font* g_nickname_font, const TTF_Font* g_level_font);

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
