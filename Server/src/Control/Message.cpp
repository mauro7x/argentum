#include "../../includes/Control/Message.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Message::Message(std::string from_nickname, std::string message,
                 MessageType message_type)
    : from_nickname(from_nickname),
      message(message),
      message_type(message_type) {}

bool Message::send(const InstanceId sender, const SocketWrapper& peer) {
    try {
        peer << (uint8_t)MESSAGE_OPCODE;
        peer << (uint8_t)message_type;
        peer << from_nickname;
        peer << message;
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}