#include <cstdlib>

#include "../../includes/Model/Position.h"

#define DEFAULT_ORIENTATION DOWN_ORIENTATION

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

void Position::move(const Orientation& orientation) {
    if (!this->map_container[id_map].moveOcuppant(this->x, this->y, orientation))
        throw CollisionWhileMovingException();

    this->orientation = orientation;

    switch (orientation) {
        case UP_ORIENTATION:
            this->y -= 1;
            break;
        
        case DOWN_ORIENTATION:
            this->y += 1;
            break;

        case RIGHT_ORIENTATION:
            this->x += 1;
            break;

        case LEFT_ORIENTATION:
            this->x -= 1;
            break;
    }
}

void Position::fillBroadcastData(PlayerData& data) const {
    data.basic_data.x_tile = this->x;
    data.basic_data.y_tile = this->y;
    data.basic_data.orientation = this->orientation;
}

const char* CollisionWhileMovingException::what() const noexcept {
    return "No puedes moverte en esa dirección.";
}
