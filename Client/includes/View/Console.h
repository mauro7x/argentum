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
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/JSON.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../defs.h"
#include "../paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "HUDComponent.h"
#include "Renderer.h"
#include "Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#define CONSOLE_OUTPUT_FONT FONT_SANFORD_FP
#define CONSOLE_INPUT_FONT FONT_SANFORD_FP
#define CONSOLE_CURSOR_FONT FONT_SANFORD_FP

#define INPUT_MAX_SIZE 256
#define CURSOR_TIME_ANIMATION 500
#define ITERATIONS_TO_SWITCH_CURSOR (CURSOR_TIME_ANIMATION / RATE)

#define ERROR_MSG_COLOR \
    { 199, 12, 12, 255 }
#define INFO_MSG_COLOR \
    { 255, 255, 255, 255 }
#define SUCCESS_MSG_COLOR \
    { 120, 235, 91, 255 }
#define LIST_MSG_COLOR \
    { 247, 223, 134, 255 }
#define PRIVATE_MSG_COLOR \
    { 250, 122, 199, 255 }
#define USER_CMD_MSG_COLOR \
    { 110, 110, 110, 255 }
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

enum MessageType {
    ERROR_MSG_TYPE,
    INFO_MSG_TYPE,
    SUCCESS_MSG_TYPE,
    LIST_MSG_TYPE,
    PRIVATE_MSG_TYPE,
    USER_CMD_MSG_TYPE,
    N_MSG_TYPES
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Console : public HUDComponent {
   private:
    // Flags internos
    bool input_enabled = false;
    bool input_changed = false;
    bool show_cursor = false;
    int cursor_cooldown = ITERATIONS_TO_SWITCH_CURSOR;

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

    // Colores de renderizado
    std::unordered_map<int, SDL_Color> colors;

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
    void add(const std::string& message, MessageType type);

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

    /* Libera recursos */
    virtual void free() override;

    //-------------------------------------------------------------------------

    /* Destructor */
    ~Console();
};

//-----------------------------------------------------------------------------

#endif  // __CONSOLE_H__
