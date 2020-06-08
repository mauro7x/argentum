#ifndef __UNIT_H__
#define __UNIT_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "Renderer.h"
//-----------------------------------------------------------------------------

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

//-----------------------------------------------------------------------------

class Unit {
   protected:
    const Renderer* g_renderer;

    int x_tile, y_tile; /* posición en la grilla en Tiles */
    SDL_Rect box;       /* posición y dimensión en pixeles */
    int x_vel, y_vel;   /* velocidad de movimiento actual */
    int next_x, next_y; /* valores a los que debemos llegar si nos movemos */
    Uint32 last_moved;  /* para trackear el tiempo entre movimientos */

    /* Centra la box de la entidad en el tile */
    void _centerBoxOnTile();

    /* Calcula valor de X al que hay que llegar para completar el movimiento */
    int _xValueToReach() const;

    /* Calcula valor de Y al que hay que llegar para completar el movimiento */
    int _yValueToReach() const;

   public:
    /* Constructor */
    Unit(const Renderer* renderer, const int x_tile = 0, const int y_tile = 0);

    /* Deshabilitamos el constructor por copia. */
    Unit(const Unit&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Unit& operator=(const Unit&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Unit(Unit&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Unit& operator=(Unit&& other) = delete;

    //-------------------------------------------------------------------------
    // Lo que debe poder hacer una entidad:

    /* Actualizar su posición según información del servidor */
    virtual void updatePosition(int corrected_x_tile, int corrected_y_tile);

    /* Moverse si debe hacerlo */
    virtual void move();

    /* Carga los archivos necesarios */
    virtual void loadMedia();

    /* Manejar un evento de I/O del usuario */
    virtual void handleEvent(const SDL_Event& e);

    /* Renderizarse si se encuentra dentro de la cámara */
    virtual void render() const = 0;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Unit();
};

//-----------------------------------------------------------------------------

#endif  // __UNIT_H__
