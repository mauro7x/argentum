#include "../../includes/View/Console.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Console::Console(const Renderer* renderer) : HUDComponent(renderer) {}

void Console::init(const json& config) {
    if (initialized) {
        throw Exception("Console already initialized.");
    }

    render_rect.x = config["offset"]["x"];
    render_rect.y = config["offset"]["y"];
    render_rect.w = config["w"];
    render_rect.h = config["h"];

    initialized = true;
}

void Console::loadMedia() {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    // Cargar media
    base.loadFromFile(g_renderer, HUD_CONSOLE_BASE_FP);
}

void Console::render() const {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    // Renderizar la base
    SDL_Rect render_quad = render_rect;
    g_renderer->render(base.getTexture(), &render_quad);
}

void Console::free() {}

Console::~Console() {}

//-----------------------------------------------------------------------------
