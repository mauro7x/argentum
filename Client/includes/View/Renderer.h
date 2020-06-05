#ifndef __RENDERER_H__
#define __RENDERER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "../../../Common/includes/Exceptions/SDLException.h"
#include "Camera.h"
#include "Window.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configuración del renderer

#define VSYNC_ENABLED true
#define RENDER_DRAW_COLOR_R 0xFF
#define RENDER_DRAW_COLOR_G 0xFF
#define RENDER_DRAW_COLOR_B 0xFF
#define RENDER_DRAW_COLOR_ALPHA 0xFF
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Renderer {
   private:
    const Window& window;
    const Camera& camera;
    SDL_Renderer* renderer;

    /* Settea el color de renderer */
    void _setDrawColor() const;

   public:
    /* Constructor */
    Renderer(const Window& window, const Camera& camera);

    /* Deshabilitamos el constructor por copia. */
    Renderer(const Renderer&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Renderer& operator=(const Renderer&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Renderer(Renderer&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Renderer& operator=(Renderer&& other) = delete;

    //-------------------------------------------------------------------------

    /* Inicializa recursos */
    void init();

    /* Limpia la pantalla */
    void clearScreen() const;

    /* Presenta la pantalla */
    void presentScreen() const;

    /* Crear textura desde una surface */
    SDL_Texture* createTextureFromSurface(SDL_Surface* surface) const;

    /* Renderiza una textura */
    void render(SDL_Texture* texture, SDL_Rect* render_quad,
                const SDL_Rect* clip = NULL, double angle = 0.0,
                SDL_Point* center = NULL,
                SDL_RendererFlip flip = SDL_FLIP_NONE) const;

    /* Renderiza una textura si la misma está dentro de la camara, en cuyo caso
     * le agrega el offset de la misma */
    void renderIfVisible(SDL_Texture* texture, SDL_Rect* render_quad,
                         const SDL_Rect* clip = NULL, double angle = 0.0,
                         SDL_Point* center = NULL,
                         SDL_RendererFlip flip = SDL_FLIP_NONE) const;

    /* Libera recursos */
    void free();

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Renderer();
};

//-----------------------------------------------------------------------------

#endif  // __RENDERER_H__
