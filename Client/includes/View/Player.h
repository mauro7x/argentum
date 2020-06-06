#ifndef __PLAYER_H__
#define __PLAYER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "../paths.h"
#include "Entity.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Estados del jugador

enum PlayerState { STANDING, MOVING };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Hardcodeamos la información sobre los sprites, después será un json

#define PLAYER_SPRITE_BG_R 0
#define PLAYER_SPRITE_BG_G 0
#define PLAYER_SPRITE_BG_B 0

#define PLAYER_SPRITE_W 52
#define PLAYER_SPRITE_H 81

#define PLAYER_DOWN_ROW 0
#define PLAYER_DOWN_SPRITES 6
#define PLAYER_UP_ROW 1
#define PLAYER_UP_SPRITES 6
#define PLAYER_LEFT_ROW 2
#define PLAYER_LEFT_SPRITES 6
#define PLAYER_RIGHT_ROW 3
#define PLAYER_RIGHT_SPRITES 6

#define PLAYER_FRAMES_BEFORE_CHANGE 4 /* regula velocidad de animacion */
#define PLAYER_MOVEMENT_SPEED 200     /* en pixeles/s */
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Player : Entity {
   private:
    Texture sprite_sheet;
    PlayerState state;

    /* Para controlar los distintos frames en cada animacion */
    SDL_Rect frame_clip;
    int current_frame;
    int max_frames_current_animation;
    int row_current_animation;

    /* Mueve el jugador un tile para arriba */
    void _moveUp();

    /* Mueve el jugador un tile para abajo */
    void _moveDown();

    /* Mueve el jugador un tile para la izquierda */
    void _moveLeft();

    /* Mueve el jugador un tile para la derecha */
    void _moveRight();

    /* Verifica si el movimiento terminó */
    bool _isMovementCompleted();

   public:
    /* Constructor */
    Player(const Renderer* renderer, int x_tile = 0, int y_tile = 0);

    //-------------------------------------------------------------------------
    // Lo que debe poder hacer una entidad:

    /* Carga los archivos necesarios */
    void loadMedia() override;

    /* Maneja un evento del usuario */
    void handleEvent(const SDL_Event& e) override;

    /* Se overridea este metodo ya que la corrección debe ser brusca */
    void updatePosition(int corrected_x_tile, int corrected_y_tile) override;

    /* Se mueve si es necesario */
    void move() override;

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
