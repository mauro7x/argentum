#ifndef __HUD_COMPONENT_H__
#define __HUD_COMPONENT_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/JSON.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Renderer.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class HUDComponent {
   protected:
    bool initialized;
    const Renderer* g_renderer;

    // Offsets de renderizado
    SDL_Rect render_rect = {0};

    /* Centra una textura en un rectangulo */
    void _center(SDL_Point& texture_pos, const Texture& texture,
                 const SDL_Rect& rect);

   public:
    /* Constructor */
    HUDComponent(const Renderer* renderer);

    /* Deshabilitamos el constructor por copia. */
    HUDComponent(const HUDComponent&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    HUDComponent& operator=(const HUDComponent&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    HUDComponent(HUDComponent&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    HUDComponent& operator=(HUDComponent&& other) = delete;

    //-------------------------------------------------------------------------

    /* Inicializa recursos */
    virtual void init(const json& config) = 0;

    /* Carga los archivos necesarios */
    virtual void loadMedia() = 0;

    /* Actualiza la información que se muestra */
    virtual void update(const int it);

    /* Renderiza la consola */
    virtual void render() const = 0;

    /* Libera recursos */
    virtual void free();

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~HUDComponent();
};

//-----------------------------------------------------------------------------

#endif  // __HUD_COMPONENT_H__
