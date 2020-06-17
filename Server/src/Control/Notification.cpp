#include "../../includes/Control/Notification.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

std::unique_ptr<Notification> getNotification(bool is_reply, char opcode,
                                              uint32_t length,
                                              std::string reply,
                                              uint32_t sender_length,
                                              std::string sender) {
    if (is_reply) {
        return std::unique_ptr<Notification>(
            new NotificationReply(opcode, length, reply));
    } else if (sender_length) {
        return std::unique_ptr<Notification>(
            new NotificationMessage(sender_length, sender, length, reply));
    } else {
        return std::unique_ptr<Notification>(
            new NotificationBroadcast(length, reply));
    }
}

//-----------------------------------------------------------------------------