#ifndef __RENDERER_H__
#define __RENDERER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>

#include "../../../Common/includes/Exceptions/SDLException.h"
#include "../../../Common/includes/json.hpp"
#include "Camera.h"
#include "Window.h"
using json = nlohmann::json;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Renderer {
   private:
    const Window& window;
    const Camera& camera;
    SDL_Renderer* renderer;

    Uint8 draw_color_r, draw_color_g, draw_color_b, draw_color_a;

    /* Settea el color de renderer */
    void _setDrawColor() const;

    /* Libera recursos */
    void _free();

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
    void init(const json& config);

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

    /* Rellena el rectangulo si es visible por la camara */
    void fillQuadIfVisible(SDL_Rect* quad, int r = 0, int g = 0, int b = 0,
                           int a = 0xFF) const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Renderer();
};

//-----------------------------------------------------------------------------

#endif  // __RENDERER_H__
