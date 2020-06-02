#ifndef __ENTITIES_CONTAINER_H__
#define __ENTITIES_CONTAINER_H__

//-----------------------------------------------------------------------------
#include <vector>

#include "Entity.h"
#include "SDLException.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class EntitiesContainer {
   private:
    std::vector<Entity*> mEntities;

   public:
    /* Constructor */
    EntitiesContainer();

    /* Deshabilitamos el constructor por copia. */
    EntitiesContainer(const EntitiesContainer&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    EntitiesContainer& operator=(const EntitiesContainer&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    EntitiesContainer(EntitiesContainer&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    EntitiesContainer& operator=(EntitiesContainer&& other) = delete;

    /* Agrega una nueva entidad. Se responsabiliza por su memoria. */
    void add(Entity* new_entity);

    //-------------------------------------------------------------------------
    // Wrappers:

    /* Ordena la carga de media a sus entidades */
    void loadMedia() const;

    /* Ordena el manejo de eventos */
    void handleEvent(const SDL_Event& e) const;

    /* Ordela la acción previa al renderizado */
    void act() const;

    /* Ordena la renderización a sus entidades */
    void render() const;

    /* Libera recursos de las entidades */
    void free();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~EntitiesContainer();
};

//-----------------------------------------------------------------------------

#endif  // __ENTITIES_CONTAINER_H__
