#include "../../includes/View/Console.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ConsoleProxy::ConsoleProxy(const Renderer* renderer)
    : initialized(false), g_renderer(renderer) {}

void ConsoleProxy::init(const json& config) {
    if (initialized) {
        throw Exception("Console already initialized.");
    }

    offset_x = config["offset"]["x"];
    offset_y = config["offset"]["y"];
    w = config["w"];
    h = config["h"];

    initialized = true;
}

void ConsoleProxy::loadMedia() {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    texture.loadFromFile(g_renderer, CONSOLE_PATH + std::string("Console.png"));
}

void ConsoleProxy::render() const {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    SDL_Rect render_quad = {offset_x, offset_y, w, h};
    g_renderer->render(texture.getTexture(), &render_quad);
}

ConsoleProxy::~ConsoleProxy() {}

//-----------------------------------------------------------------------------
