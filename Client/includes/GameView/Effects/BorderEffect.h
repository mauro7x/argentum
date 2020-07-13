#ifndef __BORDER_EFFECT_H__
#define __BORDER_EFFECT_H__

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
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../SDL/Renderer.h"
#include "../../SDL/Texture.h"
#include "../../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct ActiveBorderEffect {
    bool is_new;
    size_t current_clip;

    ActiveBorderEffect() : is_new(true), current_clip(0) {}
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class BorderEffect {
   private:
    const Renderer* g_renderer;
    SDL_Rect render_box = {0};
    size_t total_clips = 0;
    std::unordered_map<size_t, Texture> clips;
    int change_every_n_frames = 1;

    // Efectos en reproducción actualmente
    std::list<ActiveBorderEffect> active_effects;

   public:
    /* Constructor */
    BorderEffect(const Renderer* g_renderer);

    /* Deshabilitamos el constructor por copia. */
    BorderEffect(const BorderEffect&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    BorderEffect& operator=(const BorderEffect&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    BorderEffect(BorderEffect&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    BorderEffect& operator=(BorderEffect&& other) = delete;

    //-----------------------------------------------------------------------------
    // Métodos de la API pública

    /* Carga la media desde un archivo de configuración */
    void loadMedia(const json& config);

    /* Agrega una ejecución del efecto */
    void add();

    /* Avanza it iteraciones los efectos */
    void act(const int it);

    /* Renderiza los efectos visibles por la cámara */
    void render() const;

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~BorderEffect();
};

//-----------------------------------------------------------------------------

#endif  // __BORDER_EFFECT_H__
