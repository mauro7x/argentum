#include "../../includes/View/ServerProxy.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ServerProxy::ServerProxy(const MapView& map) : map(map) {}

/*
bool ServerProxy::canMoveTo(const int x, const int y) const {
    if (!map.isValid(x, y)) {
        return false;
    }

    if (map.collision(x, y)) {
        return false;
    }

return true;
}
*/

ServerProxy::~ServerProxy() {}

//-----------------------------------------------------------------------------
