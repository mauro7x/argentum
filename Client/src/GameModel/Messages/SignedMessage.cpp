#include "../../../includes/GameModel/Messages/SignedMessage.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

SignedMessage::SignedMessage(const std::string& sender,
                             const std::string& content, SignedMessageType type,
                             const Color& color)
    : sender(sender), content(content), type(type), color(color) {}

void SignedMessage::update(HUD& hud) const {
    switch (type) {
        case GENERAL_SIGNED_MSG: {
            hud.addMessage(sender + ": " + content, color);
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
