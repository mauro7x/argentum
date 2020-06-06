#ifndef __HUD_H__
#define __HUD_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include <string>

#include "../paths.h"
#include "Renderer.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class HUDProxy {
   private:
    const Renderer* g_renderer;

    /* Proxy stuff */
    Texture texture;

   public:
    /* Constructor */
    HUDProxy(const Renderer* renderer);

    /* Deshabilitamos el constructor por copia. */
    HUDProxy(const HUDProxy&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    HUDProxy& operator=(const HUDProxy&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    HUDProxy(HUDProxy&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    HUDProxy& operator=(HUDProxy&& other) = delete;

    //-------------------------------------------------------------------------

    /* Carga los archivos necesarios */
    void loadMedia();

    /* Renderiza la interfaz entera */
    void render() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~HUDProxy();
};

//-----------------------------------------------------------------------------

#endif  // __HUD_H__
