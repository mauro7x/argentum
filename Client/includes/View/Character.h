#ifndef __CHARACTER_H__
#define __CHARACTER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/defs.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Unit.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Character : public Unit {
   private:
    // Datos del personaje
    std::string nickname;
    Id head_id = 0, body_id = 0;
    EquipmentData equipment = {0};

    // Texturas para el nickname
    const TTF_Font* g_nickname_font; /* no se debe cerrar */
    const TTF_Font* g_level_font;    /* no se debe cerrar */
    Texture info_nickname;
    Texture info_level;

    /* Copia la data desde el paquete recibido */
    void _copyData(const CharacterData& init_data);

   public:
    /* Constructor */
    Character(Renderer* renderer, UnitSpriteContainer* sprites,
              const TTF_Font* g_nickname_font, const TTF_Font* g_level_font);

    //-------------------------------------------------------------------------

    /* Se inicializa con el primer paquete del server */
    void init(const CharacterData& init_data);

    /* Actualizar información según lo que diga el servidor */
    void update(const CharacterData& updated_data);

    /* Renderizarse si se encuentra dentro de la cámara */
    void render() const override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Character();
};

//-----------------------------------------------------------------------------

#endif  // __CHARACTER_H__
