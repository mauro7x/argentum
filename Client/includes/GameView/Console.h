#ifndef __CONSOLE_H__
#define __CONSOLE_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <cstdint>
#include <list>
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/JSON.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../SDL/Renderer.h"
#include "../SDL/Texture.h"
#include "../colors.h"
#include "../defs.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "HUDComponent.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define CONSOLE_OUTPUT_FONT paths::asset(FONT_SANFORD_FP)
#define CONSOLE_INPUT_FONT paths::asset(FONT_SANFORD_FP)
#define CONSOLE_CURSOR_FONT paths::asset(FONT_SANFORD_FP)

#define INPUT_MAX_SIZE 256
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Console : public HUDComponent {
   private:
    // Flags internos
    bool input_enabled = false;
    bool input_changed = false;
    bool show_cursor = false;
    int cursor_cooldown = VIEWS_ITERATIONS_TO_SWITCH_CURSOR;

    // Contenido
    std::string current_input;

    // Offsets de renderizado
    SDL_Rect input_box = {0};
    SDL_Point input_pos = {0};

    SDL_Rect output_box = {0};

    // Fuentes a utilizar
    int input_fontsize = 0;
    int output_fontsize = 0;
    TTF_Font* input_font = NULL;
    TTF_Font* output_font = NULL;
    TTF_Font* cursor_font = NULL;

    // Texturas a renderizar
    Texture base;
    Texture input;
    Texture cursor;
    std::list<Texture> messages;

    /* Settea la posición de renderizado del input-text */
    void _setInputPos();

    /* Elimina mensajes que quedan por fuera de la pantalla */
    void _discardOldMessages();

    /* Resetea el cooldown del cursor switch */
    void _resetCursorCooldown();

    /* Switchea el estado actual del cursor */
    void _switchCursorVisibility();

    /* Renderiza el input-box */
    void _renderInputBox() const;

    /* Renderiza el output-box */
    void _renderOutputBox() const;

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

    /* Agrega el mensaje recibido a la consola */
    void add(const std::string& message, const Color& color);

    /* Elimina el último caracter ingresado */
    void removeChar();

    /* Obtiene el texto ingreasdo */
    std::string getInputText();

    /* Limpia el texto */
    void clearInput();

    /* Deshabilita la escritura */
    void disableInput();

    /* Actualiza la información que se muestra */
    void update(const int it) override;

    /* Renderiza la consola */
    void render() const override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Console();
};

//-----------------------------------------------------------------------------

#endif  // __CONSOLE_H__
