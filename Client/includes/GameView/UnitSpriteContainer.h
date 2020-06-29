#ifndef __UNIT_SPRITE_CONTAINER_H__
#define __UNIT_SPRITE_CONTAINER_H__

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

class UnitSpriteContainer {
   private:
    const Renderer* g_renderer;
    std::unordered_map<Id, UnitSprite> content;

    /* Carga los sprites leyendo datos del json y la textura del dirpath */
    void _loadSpritesFromJson(const json& sprites, const std::string& dirpath);

   public:
    /* Constructor */
    UnitSpriteContainer(const Renderer* renderer);

    /* Deshabilitamos el constructor por copia. */
    UnitSpriteContainer(const UnitSpriteContainer&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    UnitSpriteContainer& operator=(const UnitSpriteContainer&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    UnitSpriteContainer(UnitSpriteContainer&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    UnitSpriteContainer& operator=(UnitSpriteContainer&& other) = delete;

    //-------------------------------------------------------------------------

    /* Carga todas las texturas de las unidades */
    void loadMedia();

    /* Carga solo las texturas de cabezas */
    void loadHeadsMedia();

    /* Carga solo las texturas de cuerpos */
    void loadBodiesMedia();

    /* Obtiene el sprite correspondiente */
    const UnitSprite& get(const Id id) const;
    const UnitSprite& operator[](const Id id) const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~UnitSpriteContainer();
};

//-----------------------------------------------------------------------------
#endif  // __UNIT_SPRITE_CONTAINER_H__
