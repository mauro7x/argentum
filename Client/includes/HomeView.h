#ifndef __HOME_VIEW_H__
#define __HOME_VIEW_H__

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
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "SDL/Mixer.h"
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
    const Renderer& renderer;

    // Conexión
    SocketWrapper& socket;

    // Flags internos
    bool show_cursor = false;
    int cursor_cooldown = VIEWS_ITERATIONS_TO_SWITCH_CURSOR;

    // Widgets
    Button tutorial_btn, connect_btn;
    TextBox hostname_txtbx, port_txtbx;

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

    /* Handler de solicitud de tutorial */
    void _handleTutorialButtonPressed();

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
    HomeView(Context& current_context, const Renderer& renderer,
             SocketWrapper& socket);

    /* Destructor */
    ~HomeView();
};

//-----------------------------------------------------------------------------

#endif  // __HOME_VIEW_H__
