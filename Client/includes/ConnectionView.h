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
#include "../../Common/includes/Exceptions/Exception.h"
#include "../../Common/includes/JSON.h"
#include "../../Common/includes/Protocol.h"
#include "../../Common/includes/Socket/SocketWrapper.h"
#include "../../Common/includes/defs.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "SDL/Renderer.h"
#include "SDL/Texture.h"
#include "SDL/Widgets/Button.h"
#include "SDL/Widgets/TextBox.h"
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
#define CONNECTIONVIEW_INVALID_USERNAME_MSG "NOMBRE DE USUARIO INEXISTENTE"
#define CONNECTIONVIEW_INVALID_PASSWORD_MSG "CONTRASEÑA INCORRECTA"
#define CONNECTIONVIEW_USERNAME_CONNECTED_MSG "EL USUARIO YA ESTÁ CONECTADO"
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
    bool show_cursor = false;
    int cursor_cooldown = VIEWS_ITERATIONS_TO_SWITCH_CURSOR;

    // Widgets
    Button signup_btn, play_btn;
    TextBox username_txtbx, password_txtbx;

    // Offsets de renderizado
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
    Texture cursor;

    // Texturas dinámicas
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

    /* Envia una solicitud de login al servidor y aguarda respuesta */
    void _tryToConnect();

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
