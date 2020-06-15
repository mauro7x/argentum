#ifndef __CONSOLE_H__
#define __CONSOLE_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include <string>

#include "../../../Common/includes/JSON.h"
#include "../paths.h"
#include "Renderer.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ConsoleProxy {
   private:
    bool initialized;
    const Renderer* g_renderer;
    int offset_x, offset_y;
    int w, h;

    /* Proxy stuff */
    Texture texture;

   public:
    /* Constructor */
    ConsoleProxy(const Renderer* renderer);

    /* Deshabilitamos el constructor por copia. */
    ConsoleProxy(const ConsoleProxy&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ConsoleProxy& operator=(const ConsoleProxy&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ConsoleProxy(ConsoleProxy&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ConsoleProxy& operator=(ConsoleProxy&& other) = delete;

    //-------------------------------------------------------------------------

    /* Inicializa recursos */
    void init(const json& config);

    /* Carga los archivos necesarios */
    void loadMedia();

    /* Renderiza la interfaz entera */
    void render() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ConsoleProxy();
};

//-----------------------------------------------------------------------------

#endif  // __CONSOLE_H__
