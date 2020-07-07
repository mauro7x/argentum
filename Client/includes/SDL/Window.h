#ifndef __WINDOW_H__
#define __WINDOW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Window {
   private:
    bool initialized = false;
    SDL_Window* window = NULL;
    int w = 0, h = 0;
    SDL_Surface* window_icon = NULL;

    //-----------------------------------------------------------------------------
    // Métodos privados

    //-----------------------------------------------------------------------------

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
    void init(bool fullscreen, int w, int h, const char* title);

    /* Devuelve el puntero a la window actual (no debe liberarse) */
    SDL_Window* getWindow();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Window();
};

//-----------------------------------------------------------------------------

#endif  // __WINDOW_H__
