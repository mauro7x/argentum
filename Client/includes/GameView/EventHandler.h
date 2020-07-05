#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
#include <stdio.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <atomic>
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/BlockingQueue.h"
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../GameModel/Commands/Command.h"
#include "../GameModel/Commands/Commands.h"
#include "../GameModel/InputParser.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../SDL/Mixer.h"
#include "../colors.h"
#include "../defs.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Camera.h"
#include "HUD.h"
#include "MapView.h"
#include "Selection.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

enum Event {
    /* Evento que no nos interesa */
    INVALID_EV = 0,

    /* Movimiento */
    START_MOVING_UP_EV,
    START_MOVING_DOWN_EV,
    START_MOVING_LEFT_EV,
    START_MOVING_RIGHT_EV,
    STOP_MOVING_EV,

    /* Teclas especiales */
    GRAB_ITEM_EV,
    THROW_ITEM_EV,

    /* Interacción con la consola */
    START_INPUT_EV,
    TEXT_INPUT_EV,
    DELETE_CHAR_EV,
    STOP_INPUT_EV,

    /* Clicks del usuario */
    CAMERA_SINGLE_CLICK_EV,
    INVENTORY_SINGLE_CLICK_EV,
    INVENTORY_DOUBLE_CLICK_EV,

    /* Cierre del juego */
    EXIT_EV
};

enum Key {
    UNMAPPED_KEY = 0,
    UP_KEY,
    DOWN_KEY,
    LEFT_KEY,
    RIGHT_KEY,
    ENTER_KEY,
    GRAB_THROW_KEY,
    DELETE_KEY,
    ESC_KEY
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class EventHandler {
   private:
    // Comunicación entre hilos
    std::atomic_bool& exit;            /* flag de ejecución */
    BlockingQueue<Command*>& commands; /* cola de comandos */

    // Componentes externos para el handleo de ciertos eventos
    HUD& hud;
    MapView& map;
    const Camera& camera;
    SDL_Rect inventory_box = {0};
    SDL_Rect camera_box = {0};
    float scale_factor_w = 0;
    float scale_factor_h = 0;

    // Flags internos
    Key key_pressed = UNMAPPED_KEY;
    bool text_input_enabled = false;
    Selection current_selection;

    // Componentes internos
    std::unordered_map<SDL_Keycode, Key> keys;
    InputParser input_parser;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Bindea las keycodes de entrada */
    void _bindKeycodes();

    /* Resetea la selección a invalida */
    void _clearSelection();

    /* Resetea la selección de NPCs */
    void _clearNPCSelection();

    /* Resetea la selección de portales */
    void _clearPortalSelection();

    /* Resetea la selección de objetos del inventario */
    void _clearItemSelection();

    /* Traduce el Keycode de SDL a una key esperada */
    Key _getKey(const SDL_Keycode& key);

    /* Traduce el evento del sistema en un evento esperado */
    Event _getEvent(const SDL_Event& e);

    /* Traduce SDL_TEXTINPUT en un evento esperado */
    Event _getTextInputEv();

    /* Traduce SDL_MOUSEBUTTONDOWN en un evento esperado */
    Event _getMouseButtonDownEv(const SDL_Event& e);

    /* Traduce SDL_KEYDOWN en un evento esperado */
    Event _getKeyDownEv(const SDL_Event& e);

    /* Traduce SDL_KEYUP en un evento esperado */
    Event _getKeyUpEv(const SDL_Event& e);

    /* Obtiene la posición del click del mouse escalada */
    SDL_Point _getClickPos(const SDL_Event& e) const;

    /* Verifica si el click ocurrió dentro del SDL_Rect provisto */
    bool _clickInside(const SDL_Rect& rect, const SDL_Event& e) const;

    /* Transforma un click recibido en el mapa del juego en un tile */
    SDL_Point _clickToTile(const SDL_Event& e) const;

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    EventHandler(std::atomic_bool& exit, BlockingQueue<Command*>& commands,
                 HUD& hud, MapView& map, const Camera& camera);

    /* Inicializa recursos */
    void init(const json& config, const float scale_factor_w,
              const float scale_factor_h);

    /* Handlea un evento de SDL */
    void handleEvent(const SDL_Event& e);

    /* Destructor */
    ~EventHandler();
};

//-----------------------------------------------------------------------------

#endif  // __EVENT_HANDLER_H__
