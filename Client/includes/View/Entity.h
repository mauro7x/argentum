#ifndef __ENTITY_H__
#define __ENTITY_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "Map.h"
#include "Renderer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Entity {
   protected:
    const Renderer* g_renderer;

    // Posicion en la grilla
    int x_tile;
    int y_tile;

    // Box de la entidad
    SDL_Rect box;

    /* Centra la box de la entidad en el tile */
    void _centerBoxOnTile();

   public:
    /* Constructor */
    Entity(const Renderer* renderer);

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

    /* Corregir su posición según información del servidor */
    void correctPosition(int corrected_x_tile, int corrected_y_tile);

    /* Carga los archivos necesarios */
    virtual void loadMedia();

    /* Manejar un evento de I/O del usuario */
    virtual void handleEvent(const SDL_Event& e);

    /* Renderizarse si se encuentra dentro de la cámara */
    virtual void render() const = 0;

    //-------------------------------------------------------------------------

    /* Destructor */
    virtual ~Entity();
};

//-----------------------------------------------------------------------------

#endif  // __ENTITY_H__
