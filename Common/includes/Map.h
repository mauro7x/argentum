#ifndef __MAP_H__
#define __MAP_H__

//-----------------------------------------------------------------------------
#include <exception>
#include <vector>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Exceptions/Exception.h"
#include "JSON.h"
#include "Orientation.h"
#include "Tile.h"
#include "defs.h"
#include "paths.h"
#include "types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define DROPPING_RANGE 3
#define FREE_TILE_LOOKUP_RANGE 3
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Map {
   private:
    int w = 0, h = 0;
    std::vector<Tile> tiles;

    /* Chequea si el mapa es válido. Lanza exepción si no. */
    void _checkIfValid(const json& map) const;

    /* Llena los tiles desde el json del mapa y desde los tilesets */
    void _fillTiles(const json& map, const json& tilesets);

    /* Pasa de una posición dada por (x,y) a el número de tile en 1D */
    int _tileNumber(const int x, const int y) const;

    /* Obtiene un tile modificable */
    Tile& _getTile(const int x, const int y);

    /* Devuelve si el tile es válido y es parte del mapa */
    bool _isValid(const int x, const int y) const;

    const bool _moveOccupant(Tile& from_tile, Tile& to_tile, bool is_creature);

   public:
    /* Constructor */
    Map();

    /* Deshabilitamos el constructor por copia. */
    Map(const Map&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Map& operator=(const Map&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Map(Map&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Map& operator=(Map&& other) = delete;

    //-------------------------------------------------------------------------

    /* Inicializa sus datos leyendo el json de la dirección recibida, recibiendo
     * las medidas que se esperan de cada tile */
    void init(const json& map, const json& tilesets);

    /* Devuelve la anchura en tiles */
    int getWidthTiles() const;

    /* Devuelve la altura en tiles */
    int getHeightTiles() const;

    /* Obtiene un Tile de solo lectura */
    const Tile& getTile(const int x, const int y) const;

    /* Devuelve una referencia al NPC_ID (gráfico) para escritura */
    TileId& getNPC(const int x, const int y);

    /* Devuelve una referencia al PORTAL (gráfico) para escritura */
    TileId& getPortal(int x, int y);

    const bool moveOccupant(const int x, const int y,
                            const Orientation& orientation, bool is_creature);

    /*iniciando una posicion para el character*/
    void establishEntitySpawningPosition(int& x, int& y, bool is_creature);

    /* Ocupa una celda con un ocupante */
    void occupyTile(InstanceId id, const int x, const int y);

    /* Pone un item en una celda */
    void setItem(const Id item_id, const uint32_t amount, const int x,
                 const int y);

    /*
     * Busca el tile libre de item más cercano (en la linea direccional de la
     * orientacion recibida) al correspondiente a las coordenadas x e y
     * recibidas
     *
     * Agrega dicho item a ese tile libre buscado y cambia
     * los valores de x e y con las coordenadas del mismo.
     *
     * Dicha búsqueda se realiza hasta FREE_TILE_LOOKUP_RANGE tiles aldeaños,
     * en la línea de orientación recibida.
     *
     * Lanza CouldNotFindFreeTileException si no se encuentra tile
     * libre para que el item lo ocupe dentro del rango FREE_TILE_LOOKUP_RANGE.
     */
    void addItem(const Id item_id, const uint32_t amount, int& x, int& y);

    /*
     * Lanza CouldNotFindFreeTileException si no se encuentra tile libre alguno
     * dentro del rango FREE_TILE_LOOKUP_RANGE.
     */
    void getNearestFreeTile(int& x, int& y);

    /* Devuelve si las coordenadas recibidas están dentro de una zona segura */
    const bool isSafeZone(const int x, const int y) const;

    /* Elimina al ocupante de una celda */
    void clearTileOccupant(const int x, const int y);

    void swapTileOcuppant(const int prev_x, const int prev_y, const int new_x,
                          const int new_y);

    /* Elimina el item de una celda */
    void clearTileItem(const int x, const int y);

    /* Chequear si una posicion es valida para la criatura */
    bool isPositionValidForCreature (const int x, const int y);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Map();
};

class CouldNotFindFreeTileException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

//-----------------------------------------------------------------------------

#endif  // __MAP_H__
