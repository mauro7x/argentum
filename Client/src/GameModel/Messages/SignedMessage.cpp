#include "../../../includes/GameModel/Messages/SignedMessage.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

SignedMessage::SignedMessage(const std::string& sender,
                             const std::string& content, SignedMessageType type,
                             const Color& color, InstanceId sender_id)
    : sender(sender),
      content(content),
      type(type),
      color(color),
      sender_id(sender_id) {}

void SignedMessage::update(HUD& hud, Player& player,
                           CharacterContainer& characters) const {
    switch (type) {
        case GENERAL_SIGNED_MSG: {
            player.addMessage(sender_id, content);
            characters.addMessage(sender_id, content);
            hud.addMessage("(Chat general) " + sender + ": " + content, color);
            break;
        }

        case PRIVATE_SIGNED_MSG: {
            hud.addMessage("[" + sender + ": " + content + "]", color);
            break;
        }

        default: {
            throw Exception("SignedMEssage::update: unknown message type.");
        }
    }
}

SignedMessage::~SignedMessage() {}

//-----------------------------------------------------------------------------
