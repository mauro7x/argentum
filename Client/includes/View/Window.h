#ifndef __WINDOW_H__
#define __WINDOW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include <mutex>
#include <string>

#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Exceptions/SDLException.h"
#include "../../../Common/includes/JSON.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Window {
   private:
    bool initialized;
    SDL_Window* window;
    std::mutex m;
    bool fullscreen_allowed;
    bool fullscreen;
    int w;
    int h;

    /* Settea fullscreen mode */
    void _fullscreenMode();

    /* Settea window mode */
    void _windowMode();

    /* Libera recursos */
    void _free();

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
    void init(const json& config);

    /* Cambia el estado de la pantalla completa */
    void fullscreenModeSwitch();

    /* Devuelve window */
    SDL_Window* getWindow();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Window();
};

//-----------------------------------------------------------------------------

#endif  // __WINDOW_H__
