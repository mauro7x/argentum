#ifndef __HOME_VIEW_H__
#define __HOME_VIEW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <iostream>
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../Common/includes/JSON.h"
#include "../../Common/includes/Socket/SocketWrapper.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "SDL/Renderer.h"
#include "SDL/Texture.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "ConstantRateFunc.h"
#include "contexts.h"
#include "defs.h"
#include "paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Fuentes
#define HOMEVIEW_INPUT_FONT FONT_SANFORD_FP
#define HOMEVIEW_CURSOR_FONT FONT_SANFORD_FP
#define HOMEVIEW_INFO_FONT FONT_FORCEDSQUARE_FP

// Lógica
#define HOMEVIEW_MAX_INPUT_SIZE 20
#define HOMEVIEW_INPUT_TEXTBOX_X_OFFSET 5
#define HOMEVIEW_CURSOR_TIME_ANIMATION 500
#define HOMEVIEW_ITERATIONS_TO_SWITCH_CURSOR \
    (HOMEVIEW_CURSOR_TIME_ANIMATION / RATE)

// Colores
#define HOMEVIEW_FONT_COLOR \
    { 0, 0, 0, 255 }
#define HOMEVIEW_ERROR_COLOR \
    { 219, 4, 4, 255 }
#define HOMEVIEW_WARNING_COLOR \
    { 255, 145, 0, 255 }

// Mensajes
#define HOMEVIEW_INVALID_INPUT_MSG "DEBES COMPLETAR AMBOS CAMPOS"
#define HOMEVIEW_CONNECTING_MSG "INTENTANDO ESTABLECER CONEXION..."
#define HOMEVIEW_ERROR_CONNECTING_MSG "NO SE PUDO ESTABLECER CONEXION"
#define HOMEVIEW_MAX_INPUT_MSG "SE ALCANZO EL LIMITE DE CARACTERES"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class HomeView : public ConstantRateFunc {
   private:
    // Contexto global actual
    Context& current_context;

    // Componentes globales
    Renderer& renderer;

    // Conexión
    SocketWrapper& socket;

    // Flags internos
    bool hostname_active = false;
    bool port_active = false;
    bool connect_button_over = false;
    bool show_cursor = false;
    int cursor_cooldown = HOMEVIEW_ITERATIONS_TO_SWITCH_CURSOR;

    // Contenido
    std::string current_hostname;
    std::string current_port;

    // Offsets de renderizado
    SDL_Rect hostname_box = {0};
    SDL_Point hostname_pos = {0};
    SDL_Rect port_box = {0};
    SDL_Point port_pos = {0};
    SDL_Rect connect_box = {0};
    SDL_Rect info_box = {0};
    SDL_Point info_pos = {0};

    // Fuentes a utilizar
    int input_fontsize = 0;
    int info_fontsize = 0;
    TTF_Font* input_font = NULL;
    TTF_Font* cursor_font = NULL;
    TTF_Font* info_font = NULL;

    // Texturas a renderizar
    Texture bg;
    Texture hostname;
    Texture hostname_inactive_box;
    Texture hostname_active_box;
    Texture port;
    Texture port_inactive_box;
    Texture port_active_box;
    Texture cursor;
    Texture connect_button;
    Texture connect_button_pressed;
    Texture info_msg;

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Inicializa recursos internos */
    void _init();

    /* Carga la media necesaria */
    void _loadMedia();

    /* Vacía la cola de eventos de SDL */
    void _processSDLEvents();

    /* Renderiza la pantalla */
    void _render() const;

    /* Iteración del loop */
    void _func(const int it) override;

    //-----------------------------------------------------------------------------
    // Auxiliares

    /* Handler de eventos de SDL */
    void _handleEvent(const SDL_Event& e);

    /* Handler de solicitud de conexión */
    void _handleConnectButtonPressed();

    /* Settea la posición de renderizado de los input_texts */
    void _setInputPos();

    /* Settea la posición de renderizado del mensaje de información */
    void _setInfoPos();

    /* Obtiene la posición del mouse */
    SDL_Point _getMousePos(const SDL_Event& e) const;

    /* Verifica si una posicion está dentro de un rectangulo */
    bool _inside(const SDL_Point& pos, const SDL_Rect& box) const;

    /* Actualiza el cooldown del cursor */
    void _updateCursorAnimation(const int it);

    /* Resetea el cooldown del cursor switch */
    void _resetCursorCooldown();

    /* Switchea el estado actual del cursor */
    void _switchCursorVisibility();

    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    HomeView(Context& current_context, Renderer& renderer,
             SocketWrapper& socket);

    /* Destructor */
    ~HomeView();
};

//-----------------------------------------------------------------------------

#endif  // __HOME_VIEW_H__
