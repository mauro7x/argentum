#ifndef __PROGRESS_BAR_H__
#define __PROGRESS_BAR_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../paths.h"
#include "Renderer.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define PROGRESS_BAR_WIDTH 227
#define PROGRESS_BAR_HEIGHT 20
#define PROGRESS_BAR_FONT_SIZE 12
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

enum ProgressBarTextType {
    NONE_PBTEXTYPE = 0,
    ABSOLUTE_PBTEXTYPE,
    PORCENTUAL_PBTEXTYPE,
    BOTH_PBTEXTYPE
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ProgressBar {
   private:
    bool initialized = false;
    const Renderer* g_renderer;

    // Color del progreso
    SDL_Color color = {0xFF, 0xFF, 0xFF, 0xFF};

    // Offsets de renderizado
    SDL_Rect render_rect = {0, 0, PROGRESS_BAR_WIDTH, PROGRESS_BAR_HEIGHT};
    SDL_Point text_pos = {0};

    // Valores
    uint32_t current_value = 0;
    uint32_t max_value = 1;

    // Fuente
    ProgressBarTextType text_type = NONE_PBTEXTYPE;
    TTF_Font* font;

    // Texturas
    Texture base;
    Texture bisel;
    Texture border;
    Texture text;

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Dibuja el progreso de la barra de forma dinámica */
    void _renderProgress() const;

    /* Centra una textura en un rectangulo */
    void _center(SDL_Point& texture_pos, const Texture& texture,
                 const SDL_Rect& rect);

    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    ProgressBar(const Renderer* g_renderer);

    /* Deshabilitamos el constructor por copia. */
    ProgressBar(const ProgressBar&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    ProgressBar& operator=(const ProgressBar&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    ProgressBar(ProgressBar&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    ProgressBar& operator=(ProgressBar&& other) = delete;

    //-----------------------------------------------------------------------------
    // Métodos de la API pública

    /* Inicializa recursos */
    void init(const SDL_Point& render_offset, uint32_t current_value,
              uint32_t max_value, const SDL_Color& color,
              ProgressBarTextType text_type = NONE_PBTEXTYPE);

    /* Carga las texturas necesarias */
    void loadMedia();

    /* Actualiza la información que se muestra */
    void update(uint32_t current_value, uint32_t max_value);

    /* Dibuja la barra */
    void render() const;

    /* Libera recursos */
    void free();

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~ProgressBar();
};

//-----------------------------------------------------------------------------

#endif  // __PROGRESS_BAR_H__
