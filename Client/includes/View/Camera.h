#ifndef __CAMERA_H__
#define __CAMERA_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/defs.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Camera {
   private:
    bool initialized = false;
    SDL_Rect box = {0};
    SDL_Point pos = {0};

   public:
    /* Constructor */
    Camera();

    /* Deshabilitamos el constructor por copia. */
    Camera(const Camera&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Camera& operator=(const Camera&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Camera(Camera&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Camera& operator=(Camera&& other) = delete;

    //-------------------------------------------------------------------------

    /* Inicializa recursos */
    void init(const json& config);

    /* Devuelve si el objeto es visible por la camara o no */
    bool isVisible(const SDL_Rect* object) const;

    /* Offset de la camara en X */
    int xOffset() const;

    /* Offset de la camara en Y */
    int yOffset() const;

    /* Centra la camara en torno al objeto */
    void center(const SDL_Rect object, const int map_width,
                const int map_height);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Camera();
};

//-----------------------------------------------------------------------------

#endif  // __CAMERA_H__
