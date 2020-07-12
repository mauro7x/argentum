#ifndef __EFFECT_H__
#define __EFFECT_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <list>
#include <string>
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/JSON.h"
#include "../../../../Common/includes/defs.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../SDL/Renderer.h"
#include "../../SDL/Texture.h"
#include "../../paths.h"
#include "../Camera.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct ActiveEffect {
    bool is_new;
    size_t current_clip;
    SDL_Point effect_pos;

    ActiveEffect(const SDL_Point& effect_pos)
        : is_new(true), current_clip(0), effect_pos(effect_pos) {}
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Effect {
   private:
    const Renderer* g_renderer;
    const Camera& camera;
    size_t total_clips = 0;
    std::unordered_map<size_t, Texture> clips;

    // Change every n frames
    int change_every_n_frames = 1;

    // Offset
    int x_offset = 0;
    int y_offset = 0;

    // Clip
    bool clip_needed = false;
    SDL_Rect clip_rect = {0};
    SDL_Rect* clip = NULL;

    // Efectos en reproducción actualmente
    std::list<ActiveEffect> active_effects;

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Calcula la posición de renderizado de un efecto */
    void _setRenderQuad(SDL_Rect& render_quad, const int x_tile,
                        const int y_tile) const;

    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    Effect(const Renderer* g_renderer, const Camera& camera);

    /* Deshabilitamos el constructor por copia. */
    Effect(const Effect&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Effect& operator=(const Effect&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Effect(Effect&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Effect& operator=(Effect&& other) = delete;

    //-----------------------------------------------------------------------------
    // Métodos de la API pública

    /* Carga la media desde un archivo de configuración */
    void loadMedia(const json& config);

    /* Agrega una ejecución del efecto */
    void add(const SDL_Point& pos);

    /* Avanza it iteraciones los efectos */
    void act(const int it);

    /* Renderiza los efectos visibles por la cámara */
    void render() const;

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~Effect();
};

//-----------------------------------------------------------------------------

#endif  // __EFFECT_H__
