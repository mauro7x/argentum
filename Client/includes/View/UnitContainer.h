#ifndef __UNIT_CONTAINER_H__
#define __UNIT_CONTAINER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <fstream>
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/DataStructs.h"
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../defs.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Renderer.h"
#include "Unit.h"
#include "UnitSpriteContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

template <class ConcreteUnit, class Data>
class UnitContainer {
   private:
    Renderer* g_renderer;
    UnitSpriteContainer* g_sprites;
    std::unordered_map<InstanceId, ConcreteUnit> content;

    // Fuentes
    TTF_Font* nickname_font = NULL;
    TTF_Font* level_font = NULL;

   public:
    /* Constructor */
    UnitContainer(Renderer* renderer, UnitSpriteContainer* sprites)
        : g_renderer(renderer), g_sprites(sprites) {}

    /* Deshabilitamos el constructor por copia. */
    UnitContainer(const UnitContainer&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    UnitContainer& operator=(const UnitContainer&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    UnitContainer(UnitContainer&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    UnitContainer& operator=(UnitContainer&& other) = delete;

    //-------------------------------------------------------------------------

    /* Carga la media necesaria */
    void loadMedia() {
        // Fuentes a utilizar
        nickname_font = TTF_OpenFont(FONT_AUGUSTA_FP, INFO_NAME_FONTSIZE);
        level_font = TTF_OpenFont(FONT_CINZELBOLD_FP, INFO_LVL_FONTSIZE);

        if (!nickname_font || !level_font) {
            throw Exception(
                "UnitContainer::loadMedia: Error opening TTF_Font/s.");
        }
    }

    /* Agrega una nueva unidad con el id y la data recibida. */
    void add(const InstanceId id, const Data& init_data) {
        if (content.count(id) > 0) {
            throw Exception("ID already taken by another unit (id repeated).");
        }

        content.emplace(std::piecewise_construct, std::forward_as_tuple(id),
                        std::forward_as_tuple(g_renderer, g_sprites,
                                              nickname_font, level_font));
        content.at(id).init(init_data);
    }

    /* Actualiza la unidad con la data recibida. */
    /* Si el id es inválido lanza una excepción informándolo. */
    void update(const InstanceId id, const Data& updated_data) {
        if (content.count(id) == 0) {
            throw Exception("Attempt to update unknown unit (id is invalid).");
        }

        content.at(id).update(updated_data);
    }

    /* Obtiene la posición de una unidad en tiles */
    SDL_Point getPos(const InstanceId id) const {
        if (content.count(id) == 0) {
            throw Exception("Attempt to update unknown unit (id is invalid).");
        }

        return content.at(id).getPos();
    }

    /* Les da un turno de acción a las unidades */
    void act(const int it) {
        for (auto i = content.begin(); i != content.end(); i++) {
            i->second.act(it);
        }
    }

    /* Renderiza la unidad si el id es válido */
    void render(const InstanceId id) const {
        if (content.count(id) > 0) {
            content.at(id).render();
        }
    }

    /* Elimina la unidad. */
    /* Si el id es inválido lanza una excepción informándolo. */
    void remove(const InstanceId id) {
        if (content.count(id) == 0) {
            throw Exception("Attempt to remove unknown unit (id is invalid).");
        }

        content.erase(id);
    }

    /* Elimina todas las unidades. */
    void clear() {
        content.clear();
    }

    /* Libera recursos */
    void free() {
        if (nickname_font) {
            TTF_CloseFont(nickname_font);
            nickname_font = NULL;
        }

        if (level_font) {
            TTF_CloseFont(level_font);
            level_font = NULL;
        }
    }

    //-------------------------------------------------------------------------

    /* Destructor */
    ~UnitContainer() {
        free();
    }
};

//-----------------------------------------------------------------------------

#endif  // __UNIT_CONTAINER_H__
