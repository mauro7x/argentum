#ifndef __TILE_H__
#define __TILE_H__

//-----------------------------------------------------------------------------
#include "SDL2/SDL.h"
#include "Texture.h"
#include "config.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define TILE_WIDTH 50
#define TILE_HEIGHT 50
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Tile {
   private:
    Texture* g_texture;
    SDL_Renderer* g_renderer;
    SDL_Rect dim;

    /* Chequea si está dentro de la región dada */
    bool _checkCollision(const SDL_Rect& box) const;

   public:
    /* Constructor */
    Tile(int x, int y, Texture* texture, SDL_Renderer* renderer);

    /* Deshabilitamos el constructor por copia. */
    Tile(const Tile&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Tile& operator=(const Tile&) = delete;

    /* Habilitamos el constructor por movimiento. */
    Tile(Tile&& other);

    /* Habilitamos el operador= para movimiento. */
    Tile& operator=(Tile&& other);

    //-------------------------------------------------------------------------

    /* Se renderiza si está dentro de la camara */
    void render(const SDL_Rect& camera) const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Tile();
};

//-----------------------------------------------------------------------------

#endif  // __TILE_H__
