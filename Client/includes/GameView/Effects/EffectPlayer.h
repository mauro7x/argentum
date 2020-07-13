#ifndef __EFFECT_PLAYER_H__
#define __EFFECT_PLAYER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Exceptions/Exception.h"
#include "../../../../Common/includes/JSON.h"
#include "../../../../Common/includes/Protocol.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../SDL/Renderer.h"
#include "../../paths.h"
#include "../Camera.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "BorderEffect.h"
#include "Effect.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class EffectPlayer {
   private:
    bool media_loaded;
    const Renderer* g_renderer;

    // Efectos del juego
    std::unordered_map<int, Effect> effects;

    // Efectos de borde
    BorderEffect be_attacked_effect, be_healed_effect;

   public:
    /* Constructor */
    EffectPlayer(const Renderer* g_renderer, const Camera& camera);

    /* Deshabilitamos el constructor por copia. */
    EffectPlayer(const EffectPlayer&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    EffectPlayer& operator=(const EffectPlayer&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    EffectPlayer(EffectPlayer&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    EffectPlayer& operator=(EffectPlayer&& other) = delete;

    //-----------------------------------------------------------------------------
    // Métodos de la API pública

    /* Carga la media necesaria */
    void loadMedia();

    /* Agrega un efecto para ser reproducido */
    void add(int event_id, const SDL_Point& pos);

    /* Avanza efectos en reproducción */
    void act(const int it);

    /* Renderiza efectos en reproducción */
    void render() const;

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~EffectPlayer();
};

//-----------------------------------------------------------------------------

#endif  // __EFFECT_PLAYER_H__
