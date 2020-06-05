#ifndef __PLAYER_H__
#define __PLAYER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "../paths.h"
#include "Entity.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Hardcodeamos la información sobre los sprites, después será un json

#define PLAYER_SPRITE_BG_R 0
#define PLAYER_SPRITE_BG_G 0
#define PLAYER_SPRITE_BG_B 0

#define PLAYER_SPRITE_W 57
#define PLAYER_SPRITE_H 98

#define PLAYER_UP_ROW 1
#define PLAYER_UP_SPRITES 6
#define PLAYER_DOWN_ROW 0
#define PLAYER_DOWN_SPRITES 6
#define PLAYER_LEFT_ROW 2
#define PLAYER_LEFT_SPRITES 5
#define PLAYER_RIGHT_ROW 3
#define PLAYER_RIGHT_SPRITES 5
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Player : Entity {
   private:
    Texture sprite_sheet;

   public:
    /* Constructor */
    Player(const Renderer* renderer, int x_tile = 0, int y_tile = 0);

    //-------------------------------------------------------------------------
    // Lo que debe poder hacer una entidad:

    /* Carga los archivos necesarios */
    void loadMedia() override;

    /* Maneja un evento del usuario */
    void handleEvent(const SDL_Event& e) override;

    /* Se mueve */
    void move();

    /* Renderizarse si está dentro de la cámara */
    void render() const override;

    /* Devuelve el box para centrar la camara */
    const SDL_Rect* getBox() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Player();
};

//-----------------------------------------------------------------------------

#endif  // __PLAYER_H__
