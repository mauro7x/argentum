#include "../../includes/View/Camera.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Camera::Camera() : x(0), y(0) {}

void Camera::init(const json& config) {
    w = config["w"];
    h = config["h"];
    offset_x = config["offset"]["x"];
    offset_y = config["offset"]["y"];
}

bool Camera::isVisible(const SDL_Rect* object) const {
    // The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // Calculate the sides of rect A
    leftA = object->x;
    rightA = object->x + object->w;
    topA = object->y;
    bottomA = object->y + object->h;

    // Calculate the sides of camera
    leftB = x;
    rightB = x + w;
    topB = y;
    bottomB = y + h;

    // If any of the sides from A are outside of camera
    if (bottomA <= topB) {
        return false;
    }

    if (topA >= bottomB) {
        return false;
    }

    if (rightA <= leftB) {
        return false;
    }

    if (leftA >= rightB) {
        return false;
    }

    // If none of the sides from A are outside camera
    return true;
}

int Camera::xOffset() const {
    return offset_x - x;
}

int Camera::yOffset() const {
    return offset_y - y;
}

void Camera::center(const SDL_Rect object, const int map_width,
                    const int map_height) {
    x = (object.x + object.w / 2) - w / 2;
    y = (object.y + object.h / 2) - h / 2;

    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    if (x > map_width - w) {
        x = map_width - w;
    }
    if (y > map_height - h) {
        y = map_height - h;
    }
}

Camera::~Camera() {}

//-----------------------------------------------------------------------------
