#ifndef __CAMERA_H__
#define __CAMERA_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/JSON.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Camera {
   private:
    bool initialized;
    int x, y;
    int w, h;
    int offset_x, offset_y;
    int tile_w, tile_h;

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
    void init(const json& config, const int tile_w, const int tile_h);

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
