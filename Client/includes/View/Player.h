#ifndef __PLAYER_H__
#define __PLAYER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Player {
   private:
    SDL_Renderer* g_renderer;

   public:
    /* Constructor */
    Player(SDL_Renderer* renderer);

    /* Deshabilitamos el constructor por copia. */
    Player(const Player&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Player& operator=(const Player&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Player(Player&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Player& operator=(Player&& other) = delete;

    //-------------------------------------------------------------------------

    /* Carga los archivos necesarios */
    void loadMedia();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Player();
};

//-----------------------------------------------------------------------------

#endif  // __PLAYER_H__
