#ifndef __PLAYER_H__
#define __PLAYER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "Predictor.h"
#include "Unit.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Player : public Unit {
   private:
    const Predictor& predictor;

   public:
    /* Constructor */
    Player(const Renderer* renderer, const UnitSpriteContainer& sprites,
           const Predictor& predictor);

    /* Maneja un evento del usuario */
    void handleEvent(const SDL_Event& e);

    //-------------------------------------------------------------------------
    // Overrides de los métodos de Unit

    /* Update según info del servidor. VER ESTE MÉTODO Y EL PREDICTOR. */
    void update(int corrected_x_tile, int corrected_y_tile) override;

    /* Acción en cada frame */
    void act() override;

    /* Renderizarse si está dentro de la cámara */
    void render() const override;

    //-------------------------------------------------------------------------

    /* Devuelve el box para centrar la camara */
    SDL_Rect getBox() const;

    /* Devuelve la posicion del jugador en tiles */
    SDL_Rect getPos() const;

    /* Destructor */
    ~Player();
};

//-----------------------------------------------------------------------------

#endif  // __PLAYER_H__
