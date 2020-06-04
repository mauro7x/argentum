#ifndef __MAP_H__
#define __MAP_H__

//-----------------------------------------------------------------------------
#include <fstream>
#include <string>
#include <vector>

#include "../../../Common/includes/Exceptions/SDLException.h"
#include "SDL2/SDL.h"
#include "Texture.h"
#include "Tile.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Tiles a utilizar.

#define TILE_TEXTURES 3
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Harcodeo el mapa. Esto se deberá procesar de algún archivo.

#define MAP_WIDTH 1200
#define MAP_HEIGHT 1200
#define MAP_X_TILES (MAP_WIDTH) / (TILE_WIDTH)
#define MAP_Y_TILES (MAP_HEIGHT) / (TILE_HEIGHT)
#define MAP_TOTAL_TILES (MAP_X_TILES) * (MAP_Y_TILES)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class MapProxy {
   private:
    SDL_Renderer* gRenderer;
    std::vector<Texture> mTileTextures;
    std::vector<Tile> mTiles;

    /* Crea las texturas */
    void _createTextures(SDL_Renderer* renderer);

    /* Carga las texturas de los tiles */
    void _loadTextures();

    /* Crea los tiles con sus tipos (parsea el mapa) */
    void _createTiles();

   public:
    /* Constructor */
    MapProxy(SDL_Renderer* renderer);

    /* Deshabilitamos el constructor por copia. */
    MapProxy(const MapProxy&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    MapProxy& operator=(const MapProxy&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    MapProxy(MapProxy&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    MapProxy& operator=(MapProxy&& other) = delete;

    //-------------------------------------------------------------------------

    /* Metodo */
    void render(const SDL_Rect& camera) const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~MapProxy();
};

//-----------------------------------------------------------------------------

#endif  // __MAP_H__
