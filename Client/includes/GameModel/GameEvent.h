#ifndef __GAME_EVENT_H__
#define __GAME_EVENT_H__

//-----------------------------------------------------------------------------
#include <SDL2/SDL.h>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include <cstdint>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../SDL/Mixer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class GameEvent {
   private:
    uint8_t event_type;
    const SDL_Point event_pos;

    //-----------------------------------------------------------------------------
    // Métodos privados
    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    GameEvent(uint8_t event_type, uint32_t x, uint32_t y);

    /* Deshabilitamos el constructor por copia. */
    GameEvent(const GameEvent&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    GameEvent& operator=(const GameEvent&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    GameEvent(GameEvent&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    GameEvent& operator=(GameEvent&& other) = delete;

    //-----------------------------------------------------------------------------

    /* Manifestación del evento en el cliente */
    void act(const SDL_Point& player_pos) const;

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~GameEvent();
};

//-----------------------------------------------------------------------------

#endif  // __GAME_EVENT_H__
