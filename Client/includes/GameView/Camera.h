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
#include "../SDL/Renderer.h"
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

    /* Obtiene los extremos visibles */
    void getVisibleRect(int& x_min, int& x_max, int& y_min, int& y_max) const;

    /* Devuelve si el objeto es visible por la camara o no */
    bool isVisible(const SDL_Rect* object) const;

    /* Renderiza una textura agregandole un offset, SEA O NO VISIBLE */
    void renderAddingOffset(const Renderer* renderer, SDL_Texture* texture,
                            SDL_Rect& render_quad,
                            const SDL_Rect* clip = NULL) const;

    /* Dibuja un rectangulo agregandole un offset, SEA O NO VISIBLE */
    void fillQuadAddingOffset(const Renderer* renderer, SDL_Rect& render_quad,
                              const SDL_Color& color = {0, 0, 0, 255}) const;

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
