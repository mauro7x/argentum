#include "../../includes/View/Camera.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Camera::Camera() : x(0), y(0) {}

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
    rightB = x + SCREEN_WIDTH;
    topB = y;
    bottomB = y + SCREEN_HEIGHT;

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
    return SCREEN_X_OFFSET - x;
}

int Camera::yOffset() const {
    return SCREEN_Y_OFFSET - y;
}

void Camera::center(const SDL_Rect* object, int map_width, int map_height) {
    x = (object->x + object->w / 2) - SCREEN_WIDTH / 2;
    y = (object->y + object->h / 2) - SCREEN_HEIGHT / 2;

    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    if (x > map_width - SCREEN_WIDTH) {
        x = map_width - SCREEN_WIDTH;
    }
    if (y > map_height - SCREEN_HEIGHT) {
        y = map_height - SCREEN_HEIGHT;
    }
}

Camera::~Camera() {}

//-----------------------------------------------------------------------------
