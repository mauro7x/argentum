#include "../../includes/GameModel/GameEvent.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

GameEvent::GameEvent(uint8_t event_type, uint32_t x, uint32_t y)
    : event_type(event_type), event_pos({(int)x, (int)y}) {}

void GameEvent::act(const SDL_Point& player_pos) const {
    // Por ahora, sólo reproducimos el sonido
    Mixer::playEventSound(event_type, player_pos, event_pos);
}

GameEvent::~GameEvent() {}

//-----------------------------------------------------------------------------
