#ifndef __WINDOW_H__
#define __WINDOW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include <string>

#include "../../../Common/includes/Exceptions/SDLException.h"
#include "../../../Common/includes/json.hpp"
using json = nlohmann::json;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Window {
   private:
    SDL_Window* window;
    int w;
    int h;

   public:
    /* Constructor */
    Window();

    /* Deshabilitamos el constructor por copia. */
    Window(const Window&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Window& operator=(const Window&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Window(Window&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Window& operator=(Window&& other) = delete;

    //-------------------------------------------------------------------------

    /* Inicializa recursos */
    void init(const json config);

    /* Devuelve window */
    SDL_Window* getWindow() const;

    /* Libera recursos */
    void free();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Window();
};

//-----------------------------------------------------------------------------

#endif  // __WINDOW_H__
