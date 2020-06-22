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

const Id Notification::getMapId() const {
    return 0;
}
