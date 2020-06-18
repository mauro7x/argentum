#include "../../includes/Control/NotificationBroadcast.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

NotificationBroadcast::NotificationBroadcast(uint32_t length,
                                             std::string broadcast)
    : length(length), broadcast(broadcast) {}

bool NotificationBroadcast::send(const SocketWrapper& peer) {
    try {
        peer << char(2);
        peer << length;
        peer << broadcast;
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}
