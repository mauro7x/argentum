#include "../../includes/Control/NotificationBroadcast.h"

#include <cstdint>
#include <vector>

typedef std::vector<uint8_t> Serialized;

NotificationBroadcast::NotificationBroadcast(InstanceId id, PlayerData& data,
                                             BroadcastType broadcast_type)
    : id(id),
      map(data.basic_data.map),
      broadcast_type(broadcast_type),
      entity_type(CHARACTER_TYPE) {
    j = data;
}

NotificationBroadcast::NotificationBroadcast(InstanceId id, CreatureData& data,
                                             BroadcastType broadcast_type)
    : id(id),
      map(data.basic_data.map),
      broadcast_type(broadcast_type),
      entity_type(CREATURE_TYPE) {
    j = data;
}

NotificationBroadcast::NotificationBroadcast(ItemData& data,
                                             BroadcastType broadcast_type)
    : id(0),
      map(data.map),
      broadcast_type(broadcast_type),
      entity_type(ITEM_TYPE) {
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

bool NotificationBroadcast::send(const InstanceId sender,
                                 const SocketWrapper& peer) {
    Serialized serialized = json::to_msgpack(j);

    fprintf(stderr, "Envio broadcast tipo %i, entity %i\n",
            this->broadcast_type, this->entity_type);

    if (sender == this->id) {
        this->entity_type = PLAYER_TYPE;
    }
    size_t send = 0;
    send = peer << (char)BROADCAST_OPCODE;
    if (!send) {
        return false;
    }
    send = peer << (char)broadcast_type;
    if (!send) {
        return false;
    }
    send = peer << (char)entity_type;
    if (!send) {
        return false;
    }
    if (this->broadcast_type == DELETE_BROADCAST) {
        send = peer << this->id;
        if (!send) {
            return false;
        }
    } else {
        send = peer << serialized;
        if (!send) {
            return false;
        }
    }
    return true;
}

const Id NotificationBroadcast::getMapId() const {
    return this->map;
}

const bool NotificationBroadcast::isBroadcast() const {
    return true;
}
