#include <vector>
#include <cstdint>

#include "../../includes/Control/NotificationBroadcast.h"

typedef std::vector<uint8_t> Serialized;

NotificationBroadcast::NotificationBroadcast(InstanceId id, 
                                             PlayerData& data, 
                                             BroadcastType broadcast_type,
                                             EntityType entity_type)
    : id(id), map(data.basic_data.map), 
      broadcast_type(broadcast_type),
      entity_type(entity_type) {
        j = data;
}

NotificationBroadcast::NotificationBroadcast(
        const NotificationBroadcast& other) {
    this->id = other.id;
    this->map = other.map;
    this->broadcast_type = other.broadcast_type;
    this->entity_type = other.entity_type;
    this->j = other.j;
}


NotificationBroadcast& NotificationBroadcast::operator=(
        const NotificationBroadcast& other) {
    this->id = other.id;
    this->map = other.map;
    this->broadcast_type = other.broadcast_type;
    this->entity_type = other.entity_type;
    this->j = other.j;
    return *this;
}

bool NotificationBroadcast::send(const SocketWrapper& peer) {
    Serialized serialized = json::to_msgpack(j);
    uint32_t size = serialized.size();
    try {
        switch(this->broadcast_type) {
            case DELETE:
                peer << (char) BROADCAST_OPCODE;
                peer << (char) broadcast_type;
                peer << (char) entity_type;
                peer << this->id;
                break;

            default:
                peer << (char) BROADCAST_OPCODE;
                peer << (char) broadcast_type;
                peer << (char) entity_type;
                peer << size;
                peer << serialized;
                break;
        }
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}

const Id NotificationBroadcast::getMapId() const {
    return this->map;
}

const bool NotificationBroadcast::isBroadcast() const {
    return true;
}