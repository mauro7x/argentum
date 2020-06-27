#ifndef __RENDERER_H__
#define __RENDERER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <cmath>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/JSON.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Window.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define RENDERER_DRAW_COLOR \
    { 0, 0, 0, 255 }
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Renderer {
   private:
    bool initialized = false;
    Window& window;
    SDL_Renderer* renderer = NULL;

    // Variables internas
    float scale_factor_w = 1.0, scale_factor_h = 1.0;
    SDL_Color draw_color = RENDERER_DRAW_COLOR;

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Settea el color de renderer */
    void _setDrawColor() const;

    /* Resizea el render_quad con el factor de escala */
    void _resize(SDL_Rect* render_quad) const;

    //-----------------------------------------------------------------------------

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
    void init(bool vsync, float scale_factor_w, float scale_factor_h);

    /* Limpia la pantalla */
    void clearScreen() const;

    /* Presenta la pantalla */
    void presentScreen() const;

    /* Crear textura desde una surface */
    SDL_Texture* createTextureFromSurface(SDL_Surface* surface) const;

    /* Renderiza una textura */
    void render(SDL_Texture* texture, SDL_Rect* render_quad,
                const SDL_Rect* clip = NULL) const;

    /* Rellena el rectangulo */
    void fillQuad(SDL_Rect* quad, SDL_Color color = {0, 0, 0, 255}) const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Renderer();
};

//-----------------------------------------------------------------------------

#endif  // __RENDERER_H__
