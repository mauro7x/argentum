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

void SignedMessage::update(HUD& hud) const {
    switch (type) {
        case GENERAL_SIGNED_MSG: {
            hud.addMessage("(Chat general) " + sender + ": " + content, color);
            fprintf(stderr, "se recibio el mensaje gral con el senderid = %u\n",
                    sender_id);
            break;
        }

        case PRIVATE_SIGNED_MSG: {
            hud.addMessage("[" + sender + ": " + content + "]", color);
            fprintf(stderr,
                    "se recibio el mensaje privado con el senderid = %u\n",
                    sender_id);
            break;
        }

        default: {
            throw Exception("SignedMEssage::update: unknown message type.");
        }
    }
}

SignedMessage::~SignedMessage() {}

//-----------------------------------------------------------------------------
