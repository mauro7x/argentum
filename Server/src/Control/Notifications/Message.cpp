#include "../../../includes/Control/Notifications/Message.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Message::Message(const std::string& from_nickname, const std::string& message,
                 MessageType message_type, InstanceId from_id)
    : from_nickname(from_nickname),
      message(message),
      message_type(message_type),
      from_id(from_id) {}

Message::~Message() {}

Message::Message(const Message& other) {
    this->from_nickname = other.from_nickname;
    this->message = other.message;
    this->message_type = other.message_type;
    this->from_id = other.from_id;
}

Message& Message::operator=(const Message& other) {
    this->from_nickname = other.from_nickname;
    this->message = other.message;
    this->message_type = other.message_type;
    this->from_id = other.from_id;
    return *this;
}

bool Message::send(const InstanceId sender, const SocketWrapper& peer) {
    try {
        peer << (uint8_t)MESSAGE_OPCODE;
        peer << (uint8_t)message_type;
        if (message_type == GENERAL_MSG) {
            peer << from_id;
        }
        peer << from_nickname;
        peer << message;
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}
