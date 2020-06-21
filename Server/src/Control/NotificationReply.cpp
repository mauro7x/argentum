#include "../../includes/Control/NotificationReply.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

NotificationReply::NotificationReply(char opcode, std::string reply)
    : opcode(opcode), reply(reply), length(reply.size()) {}

bool NotificationReply::send(const InstanceId sender, const SocketWrapper& peer) {
    ssize_t sent = 0;

    sent = (peer << (char)REPLY_OPCODE);
    if (!sent) {
        return false;
    }

    sent = (peer << opcode);
    if (!sent) {
        return false;
    }

    sent = (peer << reply);
    if (!sent) {
        return false;
    }

    return true;
}
