#ifndef __CONSOLE_H__
#define __CONSOLE_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/JSON.h"
#include "../paths.h"
#include "HUDComponent.h"
#include "Renderer.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define CONSOLE_INPUT_FONT FONT_SANFORD_FP
#define INPUT_MAX_SIZE 256
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Console : public HUDComponent {
   private:
    // Flags internos
    bool input_enabled = false;
    bool input_changed = false;

    // Contenido
    std::string current_input;

    // Offsets de renderizado
    SDL_Rect input_box = {0};
    SDL_Point input_pos = {0};

    // Fuentes a utilizar
    int input_fontsize = 0;
    TTF_Font* input_font = NULL;

    // Texturas a renderizar
    Texture base;
    Texture input;

    /* Centra verticalmente el texto en una box */
    void _center(SDL_Point& texture_pos, const Texture& texture,
                 const SDL_Rect& rect);

   public:
    /* Constructor */
    Console(const Renderer* renderer);

    //-------------------------------------------------------------------------

    /* Inicializa recursos */
    void init(const json& config) override;

    /* Carga los archivos necesarios */
    void loadMedia() override;

    /* Habilita la escritura */
    void enableInput();

    /* Agrega los caracteres al final del texto actual */
    void append(const char* text);

    /* Elimina el último caracter ingresado */
    void removeChar();

    /* Obtiene el texto ingreasdo */
    std::string getInputText();

    /* Limpia el texto */
    void clearInput();

    /* Deshabilita la escritura */
    void disableInput();

    /* Actualiza la información que se muestra */
    void update() override;

    /* Renderiza la consola */
    void render() const override;

    /* Libera recursos */
    virtual void free() override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Console();
};

//-----------------------------------------------------------------------------

#endif  // __CONSOLE_H__
