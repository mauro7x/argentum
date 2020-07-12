#ifndef __BE_HEALED_H__
#define __BE_HEALED_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <string>
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/JSON.h"
#include "../../../../Common/includes/RandomNumberGenerator.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../SDL/Renderer.h"
#include "../../SDL/Texture.h"
#include "../../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class BeHealed {
   private:
    const Renderer* g_renderer;
    SDL_Rect render_box = {0};
    size_t total_clips = 0;
    std::unordered_map<size_t, Texture> clips;
    size_t frames = 0;
    size_t clips_per_frame = 0;

    // Efecto actual
    bool is_active = false;
    bool is_new = false;
    size_t current_frame = 0;

    // Generador de números random
    RandomNumberGenerator random;

   public:
    /* Constructor */
    BeHealed(const Renderer* g_renderer);

    /* Deshabilitamos el constructor por copia. */
    BeHealed(const BeHealed&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    BeHealed& operator=(const BeHealed&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    BeHealed(BeHealed&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    BeHealed& operator=(BeHealed&& other) = delete;

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
    ~BeHealed();
};

//-----------------------------------------------------------------------------

#endif  // __BE_HEALED_H__
