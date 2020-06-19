#include "../../includes/Control/NotificationReply.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

NotificationReply::NotificationReply(char opcode, uint32_t length,
                                     std::string reply)
    : opcode(opcode), length(length), reply(reply) {}

NotificationReply& NotificationReply::operator=(NotificationReply&& other){
    this->opcode = other.opcode;
    this->length = other.length;
    this->reply = other.reply;
    return *this;
}

bool NotificationReply::send(const SocketWrapper& peer) {
    try {
        peer << char(0);
        peer << opcode;
        peer << length;
        peer << reply;
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}
