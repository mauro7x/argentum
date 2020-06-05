#ifndef __PLAYER_H__
#define __PLAYER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "Entity.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Player : Entity {
   private:
    // atributos del jugador

   public:
    /* Constructor */
    Player(SDL_Renderer* renderer, int x_tile = 0, int y_tile = 0);

    //-------------------------------------------------------------------------
    // Lo que debe poder hacer una entidad:

    /* Carga los archivos necesarios */
    void loadMedia() override;

    /* Renderizarse si está dentro de la cámara */
    void render() const override;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Player();
};

//-----------------------------------------------------------------------------

#endif  // __PLAYER_H__
