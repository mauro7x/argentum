#include "../../includes/Control/Notification.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Notification::Notification() {}
Notification::~Notification() {}
//-----------------------------------------------------------------------------

const bool Notification::isBroadcast() const {
    return false;
}

const EntityType Notification::getEntityType() const {
    throw Exception(
        "Notification::getBroadcastType: asking for broadcastType for a non "
        "broadcast notification.");
}

const Id Notification::getMapId() const {
    return 0;
}
