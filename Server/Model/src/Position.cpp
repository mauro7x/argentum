#include <cstdlib>

#include "../includes/Position.h"

#define DEFAULT_ORIENTATION DOWN

Position::Position(const int id_map, 
    const int init_x_coord,
    const int init_y_coord,
    MapContainer& map_container):
        id_map(id_map),
        x(init_x_coord),
        y(init_y_coord),
        orientation(DEFAULT_ORIENTATION),
        map_container(map_container) {}

Position::~Position() {}

const int Position::getX() const {
    return this->x;
}

const int Position::getY() const {
    return this->y;
}

const unsigned int Position::getDistance(const Position& other) const {
    return abs(this->x - other.getX()) + abs(this->y - other.getY());
}

const bool Position::move(const Orientation& orientation) {
    // IMPLEMENTAR
}

