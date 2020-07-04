#include "../../../includes/Control/Notifications/Event.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Event::Event(const Id map, const uint32_t x_coord, const uint32_t y_coord,
             EventType event_type)
    : map(map), x_coord(x_coord), y_coord(y_coord), event_type(event_type) {}

Event::~Event() {}

Event::Event(const Event& other) {
    this->map = other.map;
    this->x_coord = other.x_coord;
    this->y_coord = other.y_coord;
    this->event_type = other.event_type;
}

Event& Event::operator=(const Event& other) {
    this->map = other.map;
    this->x_coord = other.x_coord;
    this->y_coord = other.y_coord;
    this->event_type = other.event_type;
    return *this;
}

bool Event::send(const InstanceId sender, const SocketWrapper& peer) {
    try {
        peer << (uint8_t)EVENT_OPCODE;
        peer << (uint8_t)event_type;
        peer << x_coord;
        peer << y_coord;
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}

const Id Event::getMapId() const {
    return this->map;
}

const bool Event::isForEveryMap() const {
    return false;
}
