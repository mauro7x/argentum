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
    std::atomic_bool& exit;
    HUD& hud;
    BlockingQueue<Command*>& commands;
    std::unordered_map<SDL_Keycode, Key> keys;
    InputParser input_parser;

    /* Flags internos */
    Key key_pressed = UNMAPPED_KEY;
    bool text_input_enabled = false;

    /* Bindea las keycodes de entrada */
    void _bindKeycodes();

    /* Traduce el Keycode de SDL a una key esperada */
    Key _getKey(const SDL_Keycode& key);

    /* Traduce el evento del sistema en un evento esperado */
    Event _getEvent(const SDL_Event& e);

    //-------------------------------------------------------------------------
    // Manejo de eventos particulares (que justifiquen abstracción)

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    EventHandler(std::atomic_bool& exit, HUD& hud,
                 BlockingQueue<Command*>& commands);

    /* Handlea un evento de SDL */
    void handleEvent(const SDL_Event& e);

    /* Destructor */
    ~EventHandler();
};

//-----------------------------------------------------------------------------

#endif  // __EVENT_HANDLER_H__
