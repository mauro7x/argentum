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
#endif  // __ID__

enum MapLayer {
    GROUND1,
    GROUND2,
    DECORATION,
    ROOF,
    SAFEZONE,
    COLLISION,
    INDOOR,
    PORTAL,
    NPC,
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
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Map {
   private:
    int w, h;
    int tile_w, tile_h;
    std::vector<Tile> tiles;

    /* Chequea si las layers del mapa son válidas. Lanza exepción si no. */
    void _checkIfValid(const json& map_layers) const;

    /* Llena los tiles desde las layers json */
    void _fillTiles(const json& map_layers);

    /* Pasa de una posición dada por (x,y) a el número de tile en 1D */
    int _tileNumber(const int x, const int y) const;

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

    /* Inicializa sus datos desde el json recibido, recibiendo las medidas que
     * se esperan de cada tile */
    void init(const json& j_map, const int tile_w, const int tile_h);

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

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Map();
};

//-----------------------------------------------------------------------------

#endif  // __MAP_H__
