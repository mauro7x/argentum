#include "../../includes/View/Console.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Console::_center(SDL_Point& texture_pos, const Texture& texture,
                      const SDL_Rect& rect) {
    // Centramos solo verticalmente
    texture_pos.x = rect.x;
    texture_pos.y = rect.y + (rect.h - texture.getHeight()) / 2;

    // Scroll horizontal
    int excess = texture.getWidth() - rect.w;
    if (excess > 0) {
        texture_pos.x -= excess;
    }
}

void Console::_resetCursorCooldown() {
    show_cursor = true;
    cursor_cooldown = ITERATIONS_TO_SWITCH_CURSOR;
}

void Console::_switchCursorVisibility() {
    if (show_cursor) {
        show_cursor = false;
    } else {
        show_cursor = true;
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Console::Console(const Renderer* renderer) : HUDComponent(renderer) {}

void Console::init(const json& config) {
    if (initialized) {
        throw Exception("Console already initialized.");
    }

    // Offsets de renderizado
    render_rect.x = config["offset"]["x"];
    render_rect.y = config["offset"]["y"];
    render_rect.w = config["w"];
    render_rect.h = config["h"];

    // Input
    input_fontsize = config["components"]["input_box"]["fontsize"];
    input_box.x =
        (int)config["components"]["input_box"]["offset"]["x"] + render_rect.x;
    input_box.y =
        (int)config["components"]["input_box"]["offset"]["y"] + render_rect.y;
    input_box.w = (int)config["components"]["input_box"]["w"];
    input_box.h = (int)config["components"]["input_box"]["h"];

    initialized = true;
}

void Console::loadMedia() {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    // Cargar media
    base.loadFromFile(g_renderer, HUD_CONSOLE_BASE_FP);

    // Cargamos la/s fuente/s a usar
    input_font = TTF_OpenFont(CONSOLE_INPUT_FONT, input_fontsize);
    cursor_font = TTF_OpenFont(CONSOLE_CURSOR_FONT, input_fontsize);

    if (!input_font || !cursor_font) {
        throw Exception("Console::loadMedia: Error opening TTF_Font/s.");
    }

    // Cargamos la textura con texto vacío
    input.loadFromRenderedText(g_renderer, input_font, " ",
                               SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
    _center(input_pos, input, input_box);

    // Cargamos el cursor
    cursor.loadFromRenderedText(g_renderer, cursor_font, "|",
                                SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
}

void Console::enableInput() {
    SDL_StartTextInput();
    _resetCursorCooldown();
    input_enabled = true;
}

void Console::append(const char* text) {
    if (current_input.size() < INPUT_MAX_SIZE) {
        current_input += text;
        input_changed = true;
    }
}

void Console::removeChar() {
    if (!current_input.empty()) {
        current_input.pop_back();
    }

    input_changed = true;
}

std::string Console::getInputText() {
    return current_input;
}

void Console::clearInput() {
    current_input.clear();
    input_changed = true;
}

void Console::disableInput() {
    SDL_StopTextInput();
    show_cursor = false;
    input_enabled = false;
}

void Console::update(const int it) {
    if (input_enabled) {
        if (input_changed) {
            if (!current_input.empty()) {
                input.loadFromRenderedText(g_renderer, input_font,
                                           current_input.c_str(),
                                           SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
            } else {
                input.loadFromRenderedText(g_renderer, input_font, " ",
                                           SDL_Color({0xFF, 0xFF, 0xFF, 0xFF}));
            }
            _center(input_pos, input, input_box);

            _resetCursorCooldown();
            input_changed = false;
        } else {
            cursor_cooldown -= it;
            while (cursor_cooldown <= 0) {
                _switchCursorVisibility();
                cursor_cooldown += ITERATIONS_TO_SWITCH_CURSOR;
            }
        }
    }
}

void Console::render() const {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    // Renderizar la base
    SDL_Rect render_quad = render_rect;
    g_renderer->render(base.getTexture(), &render_quad);

    if (input_enabled) {
        // Renderizar el input
        render_quad = {input_pos.x, input_pos.y, input.getWidth(),
                       input.getHeight()};
        g_renderer->render(input.getTexture(), &render_quad);

        // Renderizar el cursor si corresponde
        if (show_cursor) {
            if (current_input.size()) {
                render_quad = {input_pos.x + input.getWidth(), input_pos.y,
                               cursor.getWidth(), cursor.getHeight()};
            } else {
                render_quad = {input_pos.x, input_pos.y, cursor.getWidth(),
                               cursor.getHeight()};
            }

            g_renderer->render(cursor.getTexture(), &render_quad);
        }
    }
}

void Console::free() {
    if (input_font) {
        TTF_CloseFont(input_font);
        input_font = NULL;
    }

    if (cursor_font) {
        TTF_CloseFont(cursor_font);
        cursor_font = NULL;
    }
}

Console::~Console() {
    free();
}

//-----------------------------------------------------------------------------
