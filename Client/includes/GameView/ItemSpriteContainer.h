#ifndef __ITEM_SPRITE_CONTAINER_H__
#define __ITEM_SPRITE_CONTAINER_H__

//-----------------------------------------------------------------------------
#include <fstream>
#include <string>
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../SDL/Renderer.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Sprites.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ItemSpriteContainer {
   private:
    const Renderer* g_renderer;
    std::unordered_map<Id, ItemSprite> content;

    /* Carga los sprites leyendo datos del json y la textura del dirpath */
    void _loadSpritesFromJson(const json& sprites, const std::string& dirpath);

   public:
    /* Constructor */
    ItemSpriteContainer(const Renderer* renderer);

    /* Deshabilitamos el constructor por copia. */
    ItemSpriteContainer(const ItemSpriteContainer&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ItemSpriteContainer& operator=(const ItemSpriteContainer&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ItemSpriteContainer(ItemSpriteContainer&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ItemSpriteContainer& operator=(ItemSpriteContainer&& other) = delete;

    //-------------------------------------------------------------------------

    /* Carga las texturas de los tiles */
    void loadMedia();

    /* Obtiene el sprite correspondiente */
    const ItemSprite& get(const Id id) const;
    const ItemSprite& operator[](const Id id) const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~ItemSpriteContainer();
};

//-----------------------------------------------------------------------------
#endif  // __ITEM_SPRITE_CONTAINER_H__
