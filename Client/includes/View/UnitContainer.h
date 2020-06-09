#ifndef __UNIT_CONTAINER_H__
#define __UNIT_CONTAINER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include <vector>

#include "Renderer.h"
#include "Unit.h"
#include "UnitSpriteContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Librería de json a utilizar (https://github.com/nlohmann/json).
#include "../../../Common/includes/json.hpp"
using json = nlohmann::json;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class UnitContainer {
   private:
    const Renderer* g_renderer;
    const UnitSpriteContainer& g_unit_sprites;
    std::vector<Unit*> content;
    int tile_w, tile_h;

    /* Libera recursos de las unidades */
    void _free();

   public:
    /* Constructor */
    UnitContainer(const Renderer* renderer,
                  const UnitSpriteContainer& unit_sprites);

    /* Deshabilitamos el constructor por copia. */
    UnitContainer(const UnitContainer&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    UnitContainer& operator=(const UnitContainer&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    UnitContainer(UnitContainer&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    UnitContainer& operator=(UnitContainer&& other) = delete;

    /* Inicializa recursos */
    void init(const int tile_w, const int tile_h);

    /* Crea una nueva unidad. Este método va a recibir de alguna forma el
     * paquete de update del servidor, luego veremos como. Probablemente un
     * struct con los datos que la unidad debe tener. */
    void create();  // PROXY

    /* Handler de eventos */
    void handleEvent(const SDL_Event& e) const;

    /* Destructor */
    ~UnitContainer();
};

//-----------------------------------------------------------------------------

#endif  // __UNIT_CONTAINER_H__
