#include "../../includes/Control/NotificationMessage.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

NotificationMessage::NotificationMessage(uint32_t sender_length,
                                         std::string sender,
                                         uint32_t message_length,
                                         std::string message)
    : sender_length(sender_length),
      sender(sender),
      message_length(message_length),
      message(message) {}

bool NotificationMessage::send(const SocketWrapper& peer) {
    try {
        peer << char(1);
        peer << sender_length;
        peer << sender;
        peer << message_length;
        peer << message;
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}