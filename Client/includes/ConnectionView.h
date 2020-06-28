#ifndef __CONNECTION_VIEW_H__
#define __CONNECTION_VIEW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../Common/includes/JSON.h"
#include "../../Common/includes/Socket/SocketWrapper.h"
#include "../../Common/includes/defs.h"
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
// Mensajes
#define CONNECTIONVIEW_INVALID_INPUT_MSG "DEBES COMPLETAR AMBOS CAMPOS"
#define CONNECTIONVIEW_CONNECTING_MSG "CONECTANDO..."
#define CONNECTIONVIEW_MAX_INPUT_MSG "SE ALCANZO EL LIMITE DE CARACTERES"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class ConnectionView : public ConstantRateFunc {
   private:
    // Contexto global actual
    Context& current_context;

    // Componentes globales
    Renderer& renderer;

    // Conexión
    const SocketWrapper& socket;

    // Flags internos
    bool username_active = false;
    bool password_active = false;
    bool play_button_over = false;
    bool signup_button_over = false;
    bool show_cursor = false;
    int cursor_cooldown = VIEWS_ITERATIONS_TO_SWITCH_CURSOR;

    // Contenido
    std::string current_username;
    std::string current_password;

    // Offsets de renderizado
    SDL_Rect username_box = {0};
    SDL_Point username_pos = {0};
    SDL_Rect password_box = {0};
    SDL_Point password_pos = {0};
    SDL_Rect play_box = {0};
    SDL_Rect signup_box = {0};
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
    Texture username;
    Texture username_inactive_box;
    Texture username_active_box;
    Texture password;
    Texture password_inactive_box;
    Texture password_active_box;
    Texture cursor;
    Texture play_button;
    Texture play_button_pressed;
    Texture signup_button;
    Texture signup_button_pressed;
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

    /* Handler del boton jugar */
    void _handlePlayButtonPressed();

    /* Handler del boton crear personaje */
    void _handleSignUpButtonPressed();

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
    ConnectionView(Context& current_context, Renderer& renderer,
                   const SocketWrapper& socket);

    /* Destructor */
    ~ConnectionView();
};

//-----------------------------------------------------------------------------

#endif  // ____CONNECTION_VIEW_H__
