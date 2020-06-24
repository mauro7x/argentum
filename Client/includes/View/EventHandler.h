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
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../Model/Commands/Command.h"
#include "../Model/Commands/Commands.h"
#include "../Model/InputParser.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../colors.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "HUD.h"
#include "MapView.h"
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

    /* Interacción con la consola */
    START_INPUT_EV,
    TEXT_INPUT_EV,
    DELETE_CHAR_EV,
    STOP_INPUT_EV,

    /* Clicks del usuario */
    MAP_SINGLE_CLICK_EV,
    HUD_SINGLE_CLICK_EV,
    HUD_DOUBLE_CLICK_EV,

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
    const MapView& map;
    SDL_Rect inventory_box = {0};
    SDL_Rect camera_box = {0};
    float scale_factor_w = 0;
    float scale_factor_h = 0;

    // Componentes internos
    std::unordered_map<SDL_Keycode, Key> keys;
    InputParser input_parser;

    // Flags internos
    Key key_pressed = UNMAPPED_KEY;
    bool text_input_enabled = false;

    //-------------------------------------------------------------------------
    // Métodos privados

    /* Bindea las keycodes de entrada */
    void _bindKeycodes();

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

    /* Verifica si el click ocurrió dentro del SDL_Rect provisto */
    bool _clickInside(const SDL_Rect& rect, const SDL_Event& e) const;

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    EventHandler(std::atomic_bool& exit, BlockingQueue<Command*>& commands,
                 HUD& hud, const MapView& map);

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
