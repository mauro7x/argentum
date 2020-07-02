#include "../../includes/GameModel/Event.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Event::Event(uint8_t event_type, uint32_t x, uint32_t y)
    : event_type(event_type), x(x), y(y) {}

void Event::act(int player_x_tile, int player_y_tile) const {
    // Por ahora, sólo reproducimos el sonido
}

Event::~Event() {}

//-----------------------------------------------------------------------------
