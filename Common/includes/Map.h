#ifndef __MAP_H__
#define __MAP_H__

//-----------------------------------------------------------------------------
#include <vector>

#include "Exceptions/Exception.h"
#include "JSON.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

#ifndef __TILE_ID__
#define __TILE_ID__
typedef int TileId;
#endif  // __TILE_ID__

#ifndef __ID__
#define __ID__
typedef int Id;
#endif  // __ID__

#ifndef __ORIENTATION__
#define __ORIENTATION__
enum Orientation { UP, DOWN, LEFT, RIGHT };
#endif // __ORIENTATION__

enum MapLayer {
    GROUND1_LAYER,
    GROUND2_LAYER,
    DECORATION_LAYER,
    ROOF_LAYER,
    SAFEZONE_LAYER,
    COLLISION_LAYER,
    INDOOR_LAYER,
    PORTAL_LAYER,
    NPC_LAYER,
    EXPECTED_LAYERS
};

struct Tile {
    /* Graficos */
    TileId ground_1_id;
    TileId ground_2_id;
    TileId decoration_id;
    TileId roof_id;
    TileId npc_id;

    /* Metadata */
    bool collision;
    bool safe_zone;
    bool indoor;
    bool portal;
    Id npc; /* completar esto (mau) */

    /* Ocupantes */
    int occupant_id;
    int item_id;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Map {
   private:
    int w, h;
    int tile_w, tile_h;
    std::vector<Tile> tiles;

    /* Chequea si el mapa es válido. Lanza exepción si no. */
    void _checkIfValid(const json& map) const;

    /* Llena los tiles desde el json del mapa y desde los tilesets */
    void _fillTiles(const json& map, const json& tilesets);

    /* Pasa de una posición dada por (x,y) a el número de tile en 1D */
    int _tileNumber(const int x, const int y) const;

    /* Obtiene un tile modificable */
    Tile& getTile(const int x, const int y);

    const bool _moveOcuppant(Tile& from_tile, Tile& to_tile);

   public:
    /* Constructor */
    Map();

    /* Deshabilitamos el constructor por copia. */
    Map(const Map&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Map& operator=(const Map&) = delete;

    /* Habilitamos el constructor por movimiento. */
    Map(Map&& other);

    /* Habilitamos el operador= para movimiento. */
    Map& operator=(Map&& other);

    //-------------------------------------------------------------------------

    /* Inicializa sus datos leyendo el json de la dirección recibida, recibiendo
     * las medidas que se esperan de cada tile */
    void init(const json& map, const json& tilesets, const int tile_w,
              const int tile_h);

    /* Devuelve la anchura en tiles */
    int getWidthTiles() const;

    /* Devuelve la altura en tiles */
    int getHeightTiles() const;

    /* Devuelve la anchura de un tile */
    int getTileWidth() const;

    /* Devuelve la altura de un tile */
    int getTileHeight() const;

    /* Devuelve si el tile es válido y es parte del mapa */
    bool isValid(const int x, const int y) const;

    /* Obtiene un Tile de solo lectura */
    const Tile& getTile(const int x, const int y) const;

    const bool moveOcuppant(const int x, const int y, 
                            const Orientation& orientation);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Map();
};

//-----------------------------------------------------------------------------

#endif  // __MAP_H__
