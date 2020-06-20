#ifndef __HUD_COMPONENT_H__
#define __HUD_COMPONENT_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/JSON.h"
#include "Renderer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class HUDComponent {
   protected:
    bool initialized;
    const Renderer* g_renderer;

    // Offsets de renderizado
    int offset_x, offset_y;
    int w, h;

   public:
    /* Constructor */
    HUDComponent(const Renderer* renderer)
        : initialized(false), g_renderer(renderer) {}

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

    /* Renderiza la consola */
    virtual void render() const = 0;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~HUDComponent() {}
};

//-----------------------------------------------------------------------------

#endif  // __HUD_COMPONENT_H__
