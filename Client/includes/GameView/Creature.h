#ifndef __CREATURE_H__
#define __CREATURE_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/defs.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Camera.h"
#include "Unit.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define CREATURE_NICKNAME_COLOR SDL_Color({148, 0, 0, 255})
#define CREATURE_HEALTH_BAR_W 60
#define CREATURE_HEALTH_BAR_H 8
#define CREATURE_HEALTH_BAR_COLOR SDL_Color({207, 17, 17, 255})
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Creature : public Unit {
   private:
    // Datos del monstruo
    std::string name;
    Id creature_id = 0;
    uint32_t health = 0, max_health = 0 /*, level = 0*/;

    // Texturas para el nickname
    TTF_Font* g_nickname_font; /* no se debe cerrar */
    Texture info_nickname;
    Texture info_nickname_shadow;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Copia la data desde el paquete recibido */
    void _copyData(const CreatureData& data);

    /* Renderiza la información del personaje */
    void _renderInfo() const;

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    Creature(const Renderer* renderer, const Camera& camera,
             UnitSpriteContainer* sprites, TTF_Font* g_nickname_font,
             TTF_Font* g_level_font);

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
