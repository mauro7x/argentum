#ifndef __UNIT_CONTAINER_H__
#define __UNIT_CONTAINER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
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

    /* Agrega una nueva unidad con el id y la data recibida. */
    void add(const InstanceId id, const Data& init_data) {
        if (content.count(id) > 0) {
            throw Exception("ID already taken by another unit (id repeated).");
        }

        ConcreteUnit new_unit(g_renderer, g_sprites);
        new_unit.init(init_data);
        content.emplace(id, std::move(new_unit));
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

    //-------------------------------------------------------------------------

    /* Destructor */
    ~UnitContainer() {}
};

//-----------------------------------------------------------------------------

#endif  // __UNIT_CONTAINER_H__
