#ifndef __CREATURE_CONTAINER_H__
#define __CREATURE_CONTAINER_H__

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
#include "../SDL/Renderer.h"
#include "../defs.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Camera.h"
#include "Creature.h"
#include "UnitSpriteContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class CreatureContainer {
   private:
    const Renderer* g_renderer;
    const Camera& g_camera;
    UnitSpriteContainer* g_sprites;
    std::unordered_map<InstanceId, Creature> content;

    // Fuentes
    TTF_Font* nickname_font = NULL;

   public:
    /* Constructor */
    CreatureContainer(const Renderer* renderer, const Camera& camera,
                      UnitSpriteContainer* sprites);

    /* Deshabilitamos el constructor por copia. */
    CreatureContainer(const CreatureContainer&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    CreatureContainer& operator=(const CreatureContainer&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    CreatureContainer(CreatureContainer&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    CreatureContainer& operator=(CreatureContainer&& other) = delete;

    //-----------------------------------------------------------------------------
    // Métodos de la API pública

    /* Carga la media necesaria */
    void loadMedia();

    /* Agrega una nueva unidad con el id y la data recibida. */
    void add(const InstanceId id, const CreatureData& init_data);

    /* Actualiza la unidad con la data recibida. */
    /* Si el id es inválido lanza una excepción informándolo. */
    void update(const InstanceId id, const CreatureData& updated_data);

    /* Obtiene la posición de una unidad en tiles */
    SDL_Point getPos(const InstanceId id) const;

    /* Les da un turno de acción a las unidades */
    void act(const int it);

    /* Renderiza la unidad si el id es válido */
    void render(const InstanceId id) const;

    /* Elimina la unidad. */
    /* Si el id es inválido lanza una excepción informándolo. */
    void remove(const InstanceId id);

    /* Elimina todas las unidades. */
    void clear();

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~CreatureContainer();
};

//-----------------------------------------------------------------------------

#endif  // __CREATURE_CONTAINER_H__
