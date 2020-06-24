#include "../../includes/Control/NotificationReply.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

NotificationReply::NotificationReply(uint8_t message_type, std::string reply)
    : message_type(message_type), reply(reply) {}

bool NotificationReply::send(const InstanceId sender,
                             const SocketWrapper& peer) {
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
