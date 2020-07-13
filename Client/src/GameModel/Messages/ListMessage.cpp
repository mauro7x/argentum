#include "../../../includes/GameModel/Messages/ListMessage.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ListMessage::ListMessage(const std::string& initial_msg,
                         const std::list<std::string>& list_items,
                         const Color& color)
    : initial_msg(initial_msg), list_items(list_items), color(color) {}

void ListMessage::update(HUD& hud, Player& player,
                         CharacterContainer& characters) const {
    std::string msg_to_add;
    msg_to_add += initial_msg + ": ";

    for (auto it = list_items.begin(); it != list_items.end(); it++) {
        msg_to_add += *it;
        msg_to_add += " | ";
    }

    if (!list_items.empty()) {
        // Removemos el último separador añadido

        msg_to_add.pop_back();
        msg_to_add.pop_back();
        msg_to_add.pop_back();
        msg_to_add += ".";
    }

    hud.addMessage(msg_to_add, color);
}

ListMessage::~ListMessage() {}

//-----------------------------------------------------------------------------
