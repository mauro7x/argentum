#ifndef __LOGIC_MAPS_H__
#define __LOGIC_MAPS_H__

//-----------------------------------------------------------------------------
#include <string>
#include <unordered_map>
#include <vector>
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Map.h"
#include "../../../Common/includes/MapContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class LogicMaps {
   private:
    MapContainer maps;

    //-------------------------------------------------------------------------
    // Métodos privados
    const bool _moveOccupant(Tile& from_tile, Tile& to_tile, bool is_creature);
    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    LogicMaps();

    /* Deshabilitamos el constructor por copia. */
    LogicMaps(const LogicMaps&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    LogicMaps& operator=(const LogicMaps&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    LogicMaps(LogicMaps&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    LogicMaps& operator=(LogicMaps&& other) = delete;

    /* Carga los archivos necesarios */
    void loadMedia();

    /* Agrega a maps_id los ids de los mapas existentes. */
    void getMapsId(std::vector<Id>& maps_id) const;

    /* Obtiene el mapa correspondiente modificable */
    Map& getMap(const Id id);

    /* Obtiene el mapa correspondiente para lectura */
    const Map& getMap(const Id id) const;

    /* Iniciando una posicion para el character*/
    void establishEntitySpawningPosition(const Id& map_id, int& x, int& y,
                                         bool is_creature);

    /*
     * Lanza CouldNotFindFreeTileException si no se encuentra tile libre alguno
     * dentro del rango FREE_TILE_LOOKUP_RANGE.
     */
    void getNearestFreeTile(const Id& map_id, int& x, int& y,
                            const bool is_for_item) const;

    /* Obtener el vector de ids de criaturas */
    const std::vector<Id>& getCreatures(const Id& map_id) const;

    /* Obtiene un Tile de solo lectura realizando el chequeo de los bounds */
    const Tile& getTile(const Id& map_id, const int x, const int y) const;

    /* Obtener el nombre del map */
    const std::string& getMapName(const Id& map_id) const;

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
    void addItem(const Id& map_id, const Id item_id, const uint32_t amount,
                 int& x, int& y);

    /* Elimina al ocupante de una celda */
    void clearTileOccupant(const Id& map_id, const int x, const int y);

    /* Ocupa una celda con un ocupante */
    void occupyTile(const Id& map_id, InstanceId id, const int x, const int y);

    /* Elimina el item de una celda */
    void clearTileItem(const Id& map_id, const int x, const int y);

    const bool moveOccupant(const Id& map_id, const int x, const int y,
                            const Orientation& orientation, bool is_creature);

    /* Devuelve si las coordenadas recibidas están dentro de una zona segura */
    const bool isSafeZone(const Id& map_id, const int x, const int y) const;

    /* Chequear si una posicion es valida para la criatura */
    bool isPositionValidForCreature(const Id& map_id, const int x, const int y) const;

};
//-----------------------------------------------------------------------------
#endif  // __LOGIC_MAPS_H__
