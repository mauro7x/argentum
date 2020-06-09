#ifndef __UNIT_SPRITE_CONTAINER_H__
#define __UNIT_SPRITE_CONTAINER_H__

//-----------------------------------------------------------------------------
#include <fstream>
#include <string>
#include <unordered_map>

#include "../../../Common/includes/Exceptions/Exception.h"
#include "../paths.h"
#include "Renderer.h"
#include "Sprite.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Librería de json a utilizar (https://github.com/nlohmann/json).
#include "../../../Common/includes/json.hpp"
using json = nlohmann::json;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class UnitSpriteContainer {
   private:
    const Renderer* g_renderer;
    std::unordered_map<Id, Sprite> content;

    /* Lee el archivo json del filepath especificado */
    json _loadJsonFile(std::string filepath) const;

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

    /* Carga las texturas de los tiles */
    void loadMedia();

    /* Obtiene el sprite correspondiente */
    const Sprite& get(const Id id) const;
    const Sprite& operator[](const Id id) const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~UnitSpriteContainer();
};

//-----------------------------------------------------------------------------
#endif  // __UNIT_SPRITE_CONTAINER_H__
