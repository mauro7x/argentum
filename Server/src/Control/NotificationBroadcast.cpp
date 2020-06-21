#include <vector>
#include <cstdint>

#include "../../includes/Control/NotificationBroadcast.h"

typedef std::vector<uint8_t> Serialized;

NotificationBroadcast::NotificationBroadcast(InstanceId id, PlayerData& data, BroadcastType type)
    : id(id), map(data.basic_data.map), type(type) {
        j = data;
}

bool NotificationBroadcast::send(const SocketWrapper& peer) {
    Serialized serialized = json::to_msgpack(j);
    // ssize_t size = serialized.size();
    try {
        switch(this->type) {
            case DELETE:
                peer << (char) BROADCAST_OPCODE;
                // COMPLETAR
                break;

            default:
                peer << (char) BROADCAST_OPCODE;
                peer << (char) type;
                // peer << size;
                peer << serialized;
                break;
        }
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}
