#ifndef __BE_ATTACKED_H__
#define __BE_ATTACKED_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
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

class BeAttacked {
   private:
    const Renderer* g_renderer;
    SDL_Rect render_box = {0};
    size_t total_clips = 0;
    std::unordered_map<size_t, Texture> clips;
    int change_every_n_frames = 1;

    // Current effect
    bool is_active = false;
    size_t current_frame = 0;

    //-----------------------------------------------------------------------------
    // Métodos privados

    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    BeAttacked(const Renderer* g_renderer);

    /* Deshabilitamos el constructor por copia. */
    BeAttacked(const BeAttacked&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    BeAttacked& operator=(const BeAttacked&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    BeAttacked(BeAttacked&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    BeAttacked& operator=(BeAttacked&& other) = delete;

    //-----------------------------------------------------------------------------
    // Métodos de la API pública

    /* Carga la media desde un archivo de configuración */
    void loadMedia(const json& config);

    /* Activa la reproducción del efecto */
    void add();

    /* Avanza it iteraciones el efecto actual (si existe) */
    void act(const int it);

    /* Renderiza el efecto actual (si existe)  */
    void render() const;

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~BeAttacked();
};

//-----------------------------------------------------------------------------

#endif  // __BE_ATTACKED_H__
