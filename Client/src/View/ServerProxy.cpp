#include "../../includes/View/ServerProxy.h"

//-----------------------------------------------------------------------------
// Métodos privados

bool ServerProxy::_move(const int x, const int y) {
    if (!map.isValid(this->x + x, this->y + y)) {
        return false;
    }

    if (map.collision(this->x + x, this->y + y)) {
        return false;
    }

    this->x += x;
    this->y += y;
    return true;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ServerProxy::ServerProxy(const MapView& map) : map(map), x(0), y(0) {}

bool ServerProxy::moveUp() {
    return _move(0, -1);
}

bool ServerProxy::moveDown() {
    return _move(0, 1);
}

bool ServerProxy::moveLeft() {
    return _move(-1, 0);
}

bool ServerProxy::moveRight() {
    return _move(1, 0);
}

ServerProxy::~ServerProxy() {}

//-----------------------------------------------------------------------------
