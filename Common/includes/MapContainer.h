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

    /* devuelve el tamanio del map */
    uint32_t getMapsSize() const;

     /* Agrega a maps_id los ids de los mapas existentes. */
    void getMapsId(std::vector<Id>& maps_id) const;

    /* Obtiene el mapa correspondiente modificable */
    Map& operator[](const Id id);

    /* Obtiene el mapa correspondiente para lectura */
    const Map& operator[](const Id id) const;
    
    //-------------------------------------------------------------------------

    /* Destructor */
    ~MapContainer();
};

//-----------------------------------------------------------------------------

#endif  // __MAP_CONTAINER_H__
