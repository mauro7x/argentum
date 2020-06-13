#ifndef __TILE_CONTAINER_H__
#define __TILE_CONTAINER_H__

//-----------------------------------------------------------------------------
#include <fstream>
#include <string>
#include <unordered_map>

#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/paths.h"
#include "Renderer.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#ifndef __TILE_ID__
#define __TILE_ID__
typedef int TileId;
#endif  // __TILE_ID__
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class TileContainer {
   private:
    const Renderer* g_renderer;
    std::unordered_map<TileId, Texture> content;

    /* Carga texturas para un tileset específico */
    void _loadTextures(const json& tileset);

   public:
    /* Constructor */
    TileContainer(const Renderer* renderer);

    /* Deshabilitamos el constructor por copia. */
    TileContainer(const TileContainer&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    TileContainer& operator=(const TileContainer&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    TileContainer(TileContainer&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    TileContainer& operator=(TileContainer&& other) = delete;

    //-------------------------------------------------------------------------

    /* Carga las texturas de los tiles */
    void loadMedia();

    /* Obtiene la textura correspondiente */
    const Texture& operator[](const TileId id) const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~TileContainer();
};

//-----------------------------------------------------------------------------

#endif  // __TILE_CONTAINER_H__
