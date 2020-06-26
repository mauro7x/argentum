#include "../../includes/Control/ItemBroadcast.h"

ItemBroadcast::ItemBroadcast(ItemData& data, Id map,
                             BroadcastType broadcast_type)
    : map(map),
      x_coord(data.x_tile),
      y_coord(data.y_tile),
      broadcast_type(broadcast_type),
      entity_type(ITEM_TYPE) {
    j = data;
}

ItemBroadcast::~ItemBroadcast() {}

ItemBroadcast::ItemBroadcast(const ItemBroadcast& other) {
    this->map = other.map;
    this->broadcast_type = other.broadcast_type;
    this->entity_type = other.entity_type;
    this->j = other.j;
}

ItemBroadcast& ItemBroadcast::operator=(const ItemBroadcast& other) {
    this->map = other.map;
    this->broadcast_type = other.broadcast_type;
    this->entity_type = other.entity_type;
    this->j = other.j;
    return *this;
}

bool ItemBroadcast::send(const InstanceId sender, const SocketWrapper& peer) {
    Serialized serialized = json::to_msgpack(j);

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
        send = peer << this->x_coord;
        if (!send) {
            return false;
        }
        send = peer << this->y_coord;
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

const Id ItemBroadcast::getMapId() const {
    return this->map;
}

const bool ItemBroadcast::isBroadcast() const {
    return true;
}

const EntityType ItemBroadcast::getEntityType() const {
    return this->entity_type;
}
