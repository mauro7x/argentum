#include "../../../includes/Control/Notifications/Notification.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Notification::Notification() {}
Notification::~Notification() {}
//-----------------------------------------------------------------------------

const bool Notification::isForEveryMap() const {
    return true;
}

const EntityType Notification::getEntityType() const {
    throw Exception(
        "Notification::getBroadcastType: asking for broadcastType for a non "
        "broadcast notification.");
}

const Id Notification::getMapId() const {
    return 0;
}
