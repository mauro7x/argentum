#include "../../includes/GameView/Camera.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Camera::Camera() {}

void Camera::init(const json& config) {
    if (initialized) {
        throw Exception("Camera already initialized.");
    }

    // Offsets posicionales
    box.x = config["offset"]["x"];
    box.y = config["offset"]["y"];
    box.w = config["w"];
    box.h = config["h"];

    initialized = true;
}

bool Camera::isVisible(const SDL_Rect* object) const {
    if (!initialized) {
        throw Exception("Camera not initialized.");
    }

    // Calculamos los lados del objeto
    int leftA = object->x;
    int rightA = object->x + object->w;
    int topA = object->y;
    int bottomA = object->y + object->h;

    // Calculamos los lados de la cámara
    int leftB = pos.x;
    int rightB = pos.x + box.w;
    int topB = pos.y;
    int bottomB = pos.y + box.h;

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

    return true;
}

void Camera::renderIfVisible(const Renderer* renderer, SDL_Texture* texture,
                             SDL_Rect& render_quad,
                             const SDL_Rect* clip) const {
    if (isVisible(&render_quad)) {
        render_quad.x += (box.x - pos.x);
        render_quad.y += (box.y - pos.y);

        renderer->render(texture, &render_quad, clip);
    }
}

void Camera::fillQuadIfVisible(const Renderer* renderer, SDL_Rect& render_quad,
                               const SDL_Color& color) const {
    if (isVisible(&render_quad)) {
        render_quad.x += (box.x - pos.x);
        render_quad.y += (box.y - pos.y);

        renderer->fillQuad(&render_quad, color);
    }
}

int Camera::xOffset() const {
    if (!initialized) {
        throw Exception("Camera not initialized.");
    }

    return box.x - pos.x;
}

int Camera::yOffset() const {
    if (!initialized) {
        throw Exception("Camera not initialized.");
    }

    return box.y - pos.y;
}

void Camera::center(const SDL_Rect object, const int map_width,
                    const int map_height) {
    if (!initialized) {
        throw Exception("Camera not initialized.");
    }

    pos.x = (object.x + TILE_WIDTH / 2) - (box.w / 2);
    pos.y = (object.y + TILE_HEIGHT / 2) - (box.h / 2);

    if (pos.x < 0) {
        pos.x = 0;
    }
    if (pos.y < 0) {
        pos.y = 0;
    }
    if (pos.x > map_width - box.w) {
        pos.x = map_width - box.w;
    }
    if (pos.y > map_height - box.h) {
        pos.y = map_height - box.h;
    }
}

Camera::~Camera() {}

//-----------------------------------------------------------------------------
