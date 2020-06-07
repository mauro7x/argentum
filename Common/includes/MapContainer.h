#ifndef __MAP_CONTAINER_H__
#define __MAP_CONTAINER_H__

//-----------------------------------------------------------------------------
#include <fstream>
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Exceptions/Exception.h"
#include "paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Librería de json a utilizar (https://github.com/nlohmann/json).
#include "json.hpp"
using json = nlohmann::json;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifndef __ID__
#define __ID__
typedef int Id;
#endif  // __ID__

// proxy
typedef int Map;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class MapContainer {
   private:
    std::unordered_map<Id, Map> maps;

    /* Carga el archivo json que contiene la información sobre los mapas */
    json _loadMapsFile() const;

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

    //-------------------------------------------------------------------------

    /* Destructor */
    ~MapContainer();
};

//-----------------------------------------------------------------------------

#endif  // __MAP_CONTAINER_H__
