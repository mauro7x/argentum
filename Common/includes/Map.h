#ifndef __MAP_H__
#define __MAP_H__

//-----------------------------------------------------------------------------
#include <exception>
#include <list>
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
#define TILENUMBER(x, y) ((y * w) + x)
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
    // Contenido del mapa
    int w = 0, h = 0;        /* dimensiones */
    std::vector<Tile> tiles; /* tiles lógicos */

    // Metadata
    std::vector<Id> creatures;
    std::string name;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Chequea si el mapa es válido. Lanza exepción si no. */
    void _checkIfValid(const json& map) const;

    /* Llena los tiles desde el json del mapa y desde los tilesets */
    void _fillTiles(const json& map, const json& tilesets);

    /* Obtiene un tile modificable */
    Tile& _getTile(const int x, const int y);

    /* Obtiene un tile modificable sin chequear */
    Tile& _getTileWithoutChecks(const int x, const int y);

    /* Devuelve si el tile es válido y es parte del mapa */
    bool _isValid(const int x, const int y) const;


    //-------------------------------------------------------------------------

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

    /* Inicializa sus datos leyendo el json de la dirección recibida, recibiendo
     * las medidas que se esperan de cada tile */
    void init(const json& map, const json& tilesets, std::vector<Id>& creatures,
              std::string& name);

    //-------------------------------------------------------------------------
    // Escritura

    /* Devuelve una referencia al NPC_ID (gráfico) para escritura */
    TileId& getNPC(const int x, const int y);

    /* Devuelve una referencia al PORTAL (gráfico) para escritura */
    TileId& getPortal(int x, int y);



    /* Ocupa una celda con un ocupante */
    void occupyTile(InstanceId id, const int x, const int y);

    /* Pone un item en una celda */
    void setItem(const Id item_id, const uint32_t amount, const int x,
                 const int y);

    /* Elimina al ocupante de una celda */
    void clearTileOccupant(const int x, const int y);

    /* Elimina el item de una celda */
    void clearTileItem(const int x, const int y);

    /* Vacía el mapa de items y ocupantes */
    void clear();

    //-------------------------------------------------------------------------
    // Lectura

    /* Devuelve la anchura en tiles */
    int getWidthTiles() const;

    /* Devuelve la altura en tiles */
    int getHeightTiles() const;

    /* Devuelve si las coordenadas recibidas están dentro de una zona segura */
    const bool isSafeZone(const int x, const int y) const;

    /* Obtiene un Tile que puede modificar realizando el chequeo de los bounds */
    Tile& getTile(const int x, const int y);

    /* Obtiene un Tile de solo lectura realizando el chequeo de los bounds */
    const Tile& getTile(const int x, const int y) const;

    /* Obtiene un Tile de solo lectura SIN REALIZAR EL CHEQUEO */
    const Tile& getTileWithoutChecks(const int x, const int y) const;


    /* Obtener el vector de ids de criaturas */
    const std::vector<Id>& getCreatures() const;

    /* Obtener el nombre del map */
    const std::string& getMapName() const;

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
