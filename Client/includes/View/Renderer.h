#ifndef __RENDERER_H__
#define __RENDERER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <cmath>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/SDLException.h"
#include "../../../Common/includes/JSON.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Camera.h"
#include "Window.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Renderer {
   private:
    bool initialized;
    Window& window;
    const Camera& camera;
    SDL_Renderer* renderer;

    float scale_factor_w, scale_factor_h;
    Uint8 draw_color_r, draw_color_g, draw_color_b, draw_color_a;

    /* Settea el color de renderer */
    void _setDrawColor() const;

    /* Resizea el render_quad con el factor de escala */
    void _resize(SDL_Rect* render_quad) const;

    /* Libera recursos */
    void _free();

   public:
    /* Constructor */
    Renderer(Window& window, const Camera& camera);

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
    void init(const json& config, const float scale_factor_w,
              const float scale_factor_h);

    /* Limpia la pantalla */
    void clearScreen() const;

    /* Presenta la pantalla */
    void presentScreen() const;

    /* Crear textura desde una surface */
    SDL_Texture* createTextureFromSurface(SDL_Surface* surface) const;

    /* Renderiza una textura */
    void render(SDL_Texture* texture, SDL_Rect* render_quad,
                const SDL_Rect* clip = NULL) const;

    /* Renderiza una textura si la misma está dentro de la camara, en cuyo caso
     * le agrega el offset de la misma */
    void renderIfVisible(SDL_Texture* texture, SDL_Rect* render_quad,
                         const SDL_Rect* clip = NULL) const;

    /* Rellena el rectangulo */
    void fillQuad(SDL_Rect* quad, int r = 0, int g = 0, int b = 0,
                  int a = 0xFF) const;

    /* Rellena el rectangulo si es visible por la camara */
    void fillQuadIfVisible(SDL_Rect* quad, int r = 0, int g = 0, int b = 0,
                           int a = 0xFF) const;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Renderer();
};

//-----------------------------------------------------------------------------

#endif  // __RENDERER_H__
