#ifndef __UNIT_CONTAINER_H__
#define __UNIT_CONTAINER_H__

//-----------------------------------------------------------------------------
//#include <cstdint>
#include <fstream>
#include <unordered_map>

#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/UnitData.h"
#include "Character.h"
#include "Creature.h"
#include "Renderer.h"
#include "Unit.h"
#include "UnitSpriteContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

template <class ConcreteUnit, class Data>
class UnitContainer {
   private:
    bool initialized;
    Renderer* g_renderer;
    UnitSpriteContainer* g_sprites;
    std::unordered_map<Id, ConcreteUnit> content;
    int tile_w, tile_h;
    float tile_movement_time;

   public:
    /* Constructor */
    UnitContainer(Renderer* renderer, UnitSpriteContainer* sprites)
        : initialized(false),
          g_renderer(renderer),
          g_sprites(sprites),
          tile_w(0),
          tile_h(0),
          tile_movement_time(0) {}

    /* Deshabilitamos el constructor por copia. */
    UnitContainer(const UnitContainer&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    UnitContainer& operator=(const UnitContainer&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    UnitContainer(UnitContainer&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    UnitContainer& operator=(UnitContainer&& other) = delete;

    //-------------------------------------------------------------------------

    /* Inicializa recursos */
    void init(const int tile_w, const int tile_h,
              const float tile_movement_time) {
        if (initialized) {
            throw Exception("Container already initialized.");
        }

        this->tile_w = tile_w;
        this->tile_h = tile_h;
        this->tile_movement_time = tile_movement_time;

        initialized = true;
    }

    /* Agrega una nueva unidad con el id y la data recibida. */
    void add(const Id id, const Data& init_data) {
        if (!initialized) {
            throw Exception("Container not initialized.");
        }

        if (content.count(id) > 0) {
            throw Exception("ID already taken by another unit (id repeated).");
        }

        ConcreteUnit new_unit(g_renderer, g_sprites, tile_w, tile_h,
                              tile_movement_time);
        new_unit.init(init_data);
        content.emplace(id, std::move(new_unit));
    }

    /* Actualiza la unidad con la data recibida. */
    /* Si el id es inválido lanza una excepción informándolo. */
    void update(const Id id, const Data& updated_data) {
        if (!initialized) {
            throw Exception("Container not initialized.");
        }

        if (content.count(id) == 0) {
            throw Exception("Attempt to update unknown unit (id is invalid).");
        }

        content.at(id).update(updated_data);
    }

    /* Renderiza la unidad si el id es válido */
    void render(const Id id) const {
        if (!initialized) {
            throw Exception("Container not initialized.");
        }

        if (content.count(id) > 0) {
            content.at(id).render();
        }
    }

    /* Elimina la unidad. */
    /* Si el id es inválido lanza una excepción informándolo. */
    void remove(const Id id) {
        if (!initialized) {
            throw Exception("Container not initialized.");
        }

        if (content.count(id) == 0) {
            throw Exception("Attempt to remove unknown unit (id is invalid).");
        }

        content.erase(id);
    }

    /* Elimina todas las unidades. */
    void clear() {
        if (!initialized) {
            throw Exception("Container not initialized.");
        }

        content.clear();
    }

    //-------------------------------------------------------------------------

    /* Destructor */
    ~UnitContainer() {}
};

//-----------------------------------------------------------------------------

#endif  // __UNIT_CONTAINER_H__
