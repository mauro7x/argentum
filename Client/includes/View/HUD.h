#ifndef __HUD_H__
#define __HUD_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include <string>

#include "../../../Common/includes/JSON.h"
#include "../paths.h"
#include "Renderer.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class HUDProxy {
   private:
    bool initialized;
    const Renderer* g_renderer;
    int offset_x, offset_y;
    int w, h;

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

    /* Inicializa recursos */
    void init(const json& config);

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
