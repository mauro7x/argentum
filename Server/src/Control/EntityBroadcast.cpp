#include "../../includes/Control/EntityBroadcast.h"

#include <cstdint>
#include <vector>

EntityBroadcast::EntityBroadcast(InstanceId id, PlayerData& data,
                                             BroadcastType broadcast_type)
    : id(id),
      map(data.basic_data.map),
      broadcast_type(broadcast_type),
      entity_type(CHARACTER_TYPE) {
    j = data;
}

EntityBroadcast::~EntityBroadcast() {}

EntityBroadcast::EntityBroadcast(InstanceId id, CreatureData& data,
                                             BroadcastType broadcast_type)
    : id(id),
      map(data.basic_data.map),
      broadcast_type(broadcast_type),
      entity_type(CREATURE_TYPE) {
    j = data;
}

EntityBroadcast::EntityBroadcast(
    const EntityBroadcast& other) {
    this->id = other.id;
    this->map = other.map;
    this->broadcast_type = other.broadcast_type;
    this->entity_type = other.entity_type;
    this->j = other.j;
}

EntityBroadcast& EntityBroadcast::operator=(
    const EntityBroadcast& other) {
    this->id = other.id;
    this->map = other.map;
    this->broadcast_type = other.broadcast_type;
    this->entity_type = other.entity_type;
    this->j = other.j;
    return *this;
}

bool EntityBroadcast::send(const InstanceId sender,
                                 const SocketWrapper& peer) {
    Serialized serialized = json::to_msgpack(j);

    // fprintf(stderr, "Envio broadcast tipo %i, entity %i\n",
    // this->broadcast_type, this->entity_type);

    if (sender == this->id) {
        this->entity_type = PLAYER_TYPE;
    }
    size_t send = 0;
    send = peer << (uint8_t)BROADCAST_OPCODE;
    if (!send) {
        return false;
    }
    send = peer << (uint8_t)broadcast_type;
    if (!send) {
        return false;
    }
    send = peer << (uint8_t)entity_type;
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

const Id EntityBroadcast::getMapId() const {
    return this->map;
}

const bool EntityBroadcast::isBroadcast() const {
    return true;
}

const EntityType EntityBroadcast::getEntityType() const {
    return this->entity_type;
}
