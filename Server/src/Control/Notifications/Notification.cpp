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

const InstanceId Notification::getSourceInstanceId() const {
    throw Exception("No source instance id for this notification.");
}

const bool Notification::isEntityBroadcast() const {
    return false;
}
