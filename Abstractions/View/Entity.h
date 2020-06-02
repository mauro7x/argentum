#ifndef __ENTITY_H__
#define __ENTITY_H__

//-----------------------------------------------------------------------------
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

/* Wrapper para entidades gráficas de SDL. */
class Entity {
   protected:
    SDL_Renderer* mRenderer;

   public:
    /* Constructor */
    Entity(SDL_Renderer* renderer);

    /* Deshabilitamos el constructor por copia. */
    Entity(const Entity&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Entity& operator=(const Entity&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Entity(Entity&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Entity& operator=(Entity&& other) = delete;

    //-------------------------------------------------------------------------
    // Lo que debe poder hacer una entidad:

    /* Inicializar la media que necesite */
    virtual void loadMedia() = 0;

    /* Manejar eventos del usuario */
    virtual void handleEvent(const SDL_Event& e);

    /* Actuar en cada frame */
    virtual void act();

    /* Renderizarse */
    virtual void render() const = 0;

    /* Liberar sus recursos (SI ES NECESARIO) */
    virtual void free() = 0;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Entity();
};

//-----------------------------------------------------------------------------

#endif  // __ENTITY_H__
