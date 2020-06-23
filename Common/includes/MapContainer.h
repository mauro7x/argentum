#ifndef __MAP_CONTAINER_H__
#define __MAP_CONTAINER_H__

//-----------------------------------------------------------------------------
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Exceptions/Exception.h"
#include "JSON.h"
#include "Map.h"
#include "defs.h"
#include "paths.h"
#include "types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class MapContainer {
   private:
    std::unordered_map<Id, Map> content;

   public:
    /* Constructor */
    MapContainer();

    /* Deshabilitamos el constructor por copia. */
    MapContainer(const MapContainer&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    MapContainer& operator=(const MapContainer&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    MapContainer(MapContainer&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    MapContainer& operator=(MapContainer&& other) = delete;

    //-------------------------------------------------------------------------

    /* Carga los mapas parseando los json correspondiente */
    void loadMaps();

    /* PROVISORIO! */
    /* Para crear el mapa de conteo de criaturas por mapa */
    std::vector<Id> getMapsId() const;

    /* Obtiene el mapa correspondiente modificable */
    Map& operator[](const Id id);

    /* Obtiene el mapa correspondiente para lectura */
    const Map& operator[](const Id id) const;

    const Id getCharacterSpawningMap() const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~MapContainer();
};

//-----------------------------------------------------------------------------

#endif  // __MAP_CONTAINER_H__
