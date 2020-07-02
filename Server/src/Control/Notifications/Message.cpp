#include "../../../includes/Control/Notifications/Message.h"

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
    
Message::Message(const Message& other) {
    this->from_nickname = other.from_nickname;
    this->message = other.message;
    this->message_type = other.message_type;
}

Message& Message::operator=(const Message& other) {
    this->from_nickname = other.from_nickname;
    this->message = other.message;
    this->message_type = other.message_type;
    return *this;
}

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