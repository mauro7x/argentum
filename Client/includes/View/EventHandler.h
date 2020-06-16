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
//#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Queue.h"
#include "../../../Common/includes/Thread.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

enum Event {
    /* Evento que no nos interesa */
    INVALID,

    /* Movimiento */
    START_MOVING_UP,
    START_MOVING_DOWN,
    START_MOVING_LEFT,
    START_MOVING_RIGHT,
    STOP_MOVING,

    /* Cierre del juego */
    EXIT
};

enum Key { UNMAPPED_KEY, UP_KEY, DOWN_KEY, LEFT_KEY, RIGHT_KEY, ESC_KEY };

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class EventHandler : public Thread {
   private:
    std::atomic_bool& view_running;
    Queue<int*>& requests;
    std::unordered_map<SDL_Keycode, Key> keys;

    /* Flags internos */
    Key key_pressed;

    /* Bindea las keycodes de entrada */
    void _bindKeycodes();

    /* Traduce el Keycode de SDL a una key esperada */
    Key _getKey(const SDL_Keycode& key);

    /* Traduce el evento del sistema en un evento esperado */
    Event _getEvent(const SDL_Event& e);

    //-------------------------------------------------------------------------
    // MANEJO DE EVENTOS PARTICULARES

    //-------------------------------------------------------------------------

   public:
    /* Constructor */
    EventHandler(std::atomic_bool& view_running, Queue<int*>& requests);

    /* Método de ejecución del hilo */
    void run() override;

    /* Destructor */
    ~EventHandler();
};

//-----------------------------------------------------------------------------

#endif  // __EVENT_HANDLER_H__
