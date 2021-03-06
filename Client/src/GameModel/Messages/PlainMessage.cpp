#include "../../../includes/GameModel/Messages/PlainMessage.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

PlainMessage::PlainMessage(const std::string& content, const Color& color)
    : content(content), color(color) {}

void PlainMessage::update(HUD& hud, Player& player,
                          CharacterContainer& characters) const {
    hud.addMessage(content, color);
}

PlainMessage::~PlainMessage() {}

//-----------------------------------------------------------------------------
