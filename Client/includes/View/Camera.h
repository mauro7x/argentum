#ifndef __CAMERA_H__
#define __CAMERA_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configuración de la camara

#define SCREEN_WIDTH 660
#define SCREEN_HEIGHT 450
#define SCREEN_X_OFFSET 0
#define SCREEN_Y_OFFSET 150
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Camera {
   private:
    int x;
    int y;

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

    /* Devuelve si el objeto es visible por la camara o no */
    bool isVisible(const SDL_Rect* object) const;

    /* Offset de la camara en X */
    int xOffset() const;

    /* Offset de la camara en Y */
    int yOffset() const;

    /* Centra la camara en torno al objeto */
    void center(const SDL_Rect* object, int map_width, int map_height);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Camera();
};

//-----------------------------------------------------------------------------

#endif  // __CAMERA_H__
