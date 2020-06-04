#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../../../Common/includes/Exceptions/SDLException.h"
#include "Map.h"
#include "Texture.h"
#include "config.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Wrapper para una vista generada con SDL. */
class GameView {
   private:
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    bool sdl_running;
    bool img_running;

    //-------------------------------------------------------------------------
    // OBJETOS QUE COMPONEN LA VISTA

    //-------------------------------------------------------------------------
    // PROXY PARA EL MAPA.

    // Contenedor de mapas inicializados

    // Puntero a mapa actual (un mapa solo tiene los tiles estáticos, no se
    // incluyen acá los NPC, jugadores, o objetos sueltos en el mapa, eso será
    // extra)

    // Map map;

    //-------------------------------------------------------------------------

    // Contenedor de entidades NPC

    // Contenedor de entidades jugadores

    // Contenedor de entidades objetos sueltos

    // Un SDL_Rect que sea la camara?

    // Una unidad que represente al jugador (Player?)

    //-------------------------------------------------------------------------

    /* Inicializa recursos */
    void _init();

    /* Carga archivos media necesarios */
    void _loadMedia();

    /* Maneja un evento */
    void _handleEvent(const SDL_Event& e);

    /* Limpia la pantalla */
    void _clear() const;

    /* Acciones previas al renderizado */
    void _act() const;

    /* Actualiza la pantalla */
    void _present() const;

    /* Libera la memoria */
    void _free();

   public:
    /* Constructor */
    GameView();

    /* Deshabilitamos el constructor por copia. */
    GameView(const GameView&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    GameView& operator=(const GameView&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    GameView(GameView&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    GameView& operator=(GameView&& other) = delete;

    /* Pone a correr la vista */
    void operator()();

    /* Destructor */
    ~GameView();
};

//-----------------------------------------------------------------------------

#endif  // __GAME_VIEW_H__
