#ifndef __MAP_H__
#define __MAP_H__

//-----------------------------------------------------------------------------
#include <vector>

#include "Exceptions/Exception.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Librería de json a utilizar (https://github.com/nlohmann/json).
#include "json.hpp"
using json = nlohmann::json;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

enum MapLayer {
    GROUND,
    DECORATION,
    ROOF,
    ZONES,
    SAFEZONES,
    COLLISION,
    EXPECTED_LAYERS
};

struct Tile {
    /* Graficos */
    int ground_id;
    int decoration_id;
    int roof_id;

    /* Metadata */
    bool collision;
    bool safe_zone;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Map {
   private:
    int w, h;
    int tile_w, tile_h;
    std::vector<Tile> tiles;

    /* Chequea si las layers del mapa son válidas. Lanza exepción si no. */
    void _checkIfValid(const json map_layers) const;

    /* Llena los tiles desde las layers json */
    void _fillTiles(const json map_layers);

    /* Pasa de una posición dada por (x,y) a el número de tile en 1D */
    int _tileNumber(const int x, const int y) const;

   public:
    /* Constructor por defecto pues unordered map lo exige */
    Map();

    /* Constructor desde el json */
    Map(const json j_map);

    /* Deshabilitamos el constructor por copia. */
    Map(const Map&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Map& operator=(const Map&) = delete;

    /* Habilitamos el constructor por movimiento. */
    Map(Map&& other);

    /* Habilitamos el operador= para movimiento. */
    Map& operator=(Map&& other);

    //-------------------------------------------------------------------------

    /* Getter para el ground_id */
    int groundId(const int x, const int y) const;

    /* Getter para el decoration_id */
    int decorationId(const int x, const int y) const;

    /* Getter para el roof_id */
    int roofId(const int x, const int y) const;

    /* Chequea si hay colisión en una celda */
    bool collision(const int x, const int y) const;

    /* Chequea si hay zona segura en una celda */
    bool isSafeZone(const int x, const int y) const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Map();
};

//-----------------------------------------------------------------------------

#endif  // __MAP_H__
