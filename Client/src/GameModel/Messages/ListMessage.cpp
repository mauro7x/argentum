#include "../../../includes/GameModel/Messages/ListMessage.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ListMessage::ListMessage(const std::string& content, const Color& color)
    : content(content), color(color) {}

void ListMessage::update(HUD& hud) const {
    // implementar
}

ListMessage::~ListMessage() {}

//-----------------------------------------------------------------------------
