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
#include "Camera.h"
#include "Unit.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define CHARACTER_NICKNAME_COLOR SDL_Color({255, 255, 255, 255})
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Character : public Unit {
   private:
    // Datos del personaje
    std::string nickname;
    Id head_id = 0, body_id = 0;
    EquipmentData equipment = {0};
    uint32_t level = 0;

    // Texturas y fuentes para la info
    TTF_Font* g_nickname_font; /* no se debe cerrar */
    TTF_Font* g_level_font;    /* no se debe cerrar */
    Texture info_nickname;
    Texture info_nickname_shadow;
    Texture info_level;

    // Mensaje flotante
    TTF_Font* g_msg_font; /* no se debe cerrar */
    Texture msg;
    Texture msg_shadow;
    bool msg_active = false;
    int msg_its = 0;

    // Si es enano/gnomo usamos sus sprites
    bool is_shorter = false;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Copia la data desde el paquete recibido */
    void _copyData(const CharacterData& data);

    /* Renderiza la información del personaje */
    void _renderInfo() const;

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    Character(const Renderer* renderer, const Camera& camera,
              UnitSpriteContainer* sprites, TTF_Font* g_nickname_font,
              TTF_Font* g_level_font, TTF_Font* g_msg_font);

    //-------------------------------------------------------------------------

    /* Se inicializa con el primer paquete del server */
    void init(const CharacterData& init_data);

    /* Actualizar información según lo que diga el servidor */
    void update(const CharacterData& updated_data);

    /* Agrega mensaje flotante */
    void addMessage(const std::string& msg);

    /* Override del método de Unit para el mensaje flotante */
    void act(const int it) override;

    /* Renderizarse si se encuentra dentro de la cámara */
    void render() const override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Character();
};

//-----------------------------------------------------------------------------

#endif  // __CHARACTER_H__
