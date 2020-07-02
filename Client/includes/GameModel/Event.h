#ifndef __EVENT_H__
#define __EVENT_H__

//-----------------------------------------------------------------------------
#include <cstdint>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../SDL/Mixer.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Event {
   private:
    uint8_t event_type;
    uint32_t x;
    uint32_t y;

    //-----------------------------------------------------------------------------
    // Métodos privados
    //-----------------------------------------------------------------------------

   public:
    /* Constructor */
    Event(uint8_t event_type, uint32_t x, uint32_t y);

    /* Deshabilitamos el constructor por copia. */
    Event(const Event&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Event& operator=(const Event&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Event(Event&& other) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Event& operator=(Event&& other) = delete;

    //-----------------------------------------------------------------------------

    /* Manifestación del evento en el cliente */
    void act(int player_x_tile, int player_y_tile) const;

    //-----------------------------------------------------------------------------

    /* Destructor */
    ~Event();
};

//-----------------------------------------------------------------------------

#endif  // __EVENT_H__
