#include "../../../includes/Control/Notifications/Reply.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Reply::Reply(const uint8_t message_type, const std::string& reply)
    : message_type(message_type), reply(reply) {}

Reply::~Reply() {}

bool Reply::send(const InstanceId sender, const SocketWrapper& peer) {
    // Enviamos la notificación según el protocolo

    if (!(peer << (uint8_t)MESSAGE_OPCODE)) {
        return false;
    }

    if (!(peer << message_type)) {
        return false;
    }

    if (!(peer << reply)) {
        return false;
    }

    return true;
}
