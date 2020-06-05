#ifndef __WINDOW_H__
#define __WINDOW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "../../../Common/includes/Exceptions/SDLException.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define WINDOW_TITLE "Argentum Online"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 600
#define WINDOW_POS_X SDL_WINDOWPOS_UNDEFINED
#define WINDOW_POS_Y SDL_WINDOWPOS_UNDEFINED
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Window {
   private:
    SDL_Window* window;

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
    void init();

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
