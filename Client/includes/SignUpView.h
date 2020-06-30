#ifndef __SIGNUP_VIEW_H__
#define __SIGNUP_VIEW_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <iterator>
#include <list>
#include <string>
#include <vector>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../Common/includes/JSON.h"
#include "../../Common/includes/Protocol.h"
#include "../../Common/includes/Socket/SocketWrapper.h"
#include "../../Common/includes/defs.h"
#include "../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "GameView/UnitSpriteContainer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "SDL/Renderer.h"
#include "SDL/Texture.h"
#include "SDL/Widgets/Button.h"
#include "SDL/Widgets/SelectionInputBox.h"
#include "SDL/Widgets/TextBox.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "ConstantRateFunc.h"
#include "RoundRobinList.h"
#include "contexts.h"
#include "defs.h"
#include "paths.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Mensajes
#define SIGNUPVIEW_INVALID_INPUT_MSG "CAMPOS INCOMPLETOS"
#define SIGNUPVIEW_MAX_INPUT_MSG "LONGITUD MAXIMA ALCANZADA"

#define SIGNUPVIEW_CREATING_MSG "CREANDO PERSONAJE..."
#define SIGNUPVIEW_SUCCESS_MSG "TU PERSONAJE FUE CREADO CON ÉXITO"
#define SIGNUPVIEW_USERNAME_TAKEN "EL NOMBRE DE USUARIO ESTÁ EN USO"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

struct SelectionBoxData {
    Id id;
    SDL_Point texture_pos;
    Texture texture;

    SelectionBoxData(Id id) : id(id), texture_pos({0}) {}
};

struct RaceData : public SelectionBoxData {
    RoundRobinList<SelectionBoxData> head_ids;
    RoundRobinList<SelectionBoxData> body_ids;

    RaceData(Id id) : SelectionBoxData(id) {}
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class SignUpView : public ConstantRateFunc {
   private:
    // Contexto global actual
    Context& current_context;

    // Componentes globales
    Renderer& renderer;

    // Conexión
    const SocketWrapper& socket;

    // Sprites
    UnitSpriteContainer sprites;

    // Flags internos
    bool show_cursor = false;
    int cursor_cooldown = VIEWS_ITERATIONS_TO_SWITCH_CURSOR;

    // Widgets
    TextBox username_txtbx, password_txtbx;
    Button goback_btn, create_btn;
    SelectionInputBox race_sibx, kind_sibx, head_sibx, body_sibx;

    // Contenido
    RoundRobinList<RaceData> races_data;
    RoundRobinList<SelectionBoxData> kinds_data;

    // Offsets de renderizado
    SDL_Rect preview_tile = {0};
    SDL_Rect info_box = {0};
    SDL_Point info_pos = {0};

    // Fuentes a utilizar
    int input_fontsize = 0;
    int info_fontsize = 0;
    TTF_Font* input_font = NULL;
    TTF_Font* cursor_font = NULL;
    TTF_Font* info_font = NULL;

    // Texturas estáticas a renderizar
    Texture bg;
    Texture cursor;

    // Texturas dinámicas
    Texture info_msg;

    //-----------------------------------------------------------------------------
    // Métodos privados

    /* Inicializa recursos internos */
    void _init();

    /* Inicializa la data necesaria para la preview */
    void _loadPreviewData();

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
    void _handleCreateButtonPressed();

    /* Handler del boton crear personaje */
    void _handleGoBackButtonPressed();

    /* Intenta crear el personaje mandando el mensaje al servidor */
    void _tryToCreate();

    /* Renderiza un sprite en la ventana de preview */
    void _renderSprite(const UnitSprite& sprite) const;

    /* Settea la posición de renderizado de los input_texts */
    void _setInputPos();

    /* Settea la posición de renderizado de los selection-boxs */
    void _setSelectionBoxPos();

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
    SignUpView(Context& current_context, Renderer& renderer,
               const SocketWrapper& socket);

    /* Destructor */
    ~SignUpView();
};

//-----------------------------------------------------------------------------

#endif  // ____SIGNUP_VIEW_H__
