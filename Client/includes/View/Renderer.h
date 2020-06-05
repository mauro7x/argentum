#ifndef __RENDERER_H__
#define __RENDERER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "../../../Common/includes/Exceptions/SDLException.h"
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
// Configuración de la camara

#define SCREEN_WIDTH 660
#define SCREEN_HEIGHT 450
#define SCREEN_X_OFFSET 0
#define SCREEN_Y_OFFSET 150
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Renderer {
   private:
    Window& window;
    SDL_Renderer* renderer;

    /* Settea el color de renderer */
    void _setDrawColor() const;

   public:
    /* Constructor */
    Renderer(Window& window);

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
                SDL_Rect* clip = NULL, double angle = 0.0,
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
