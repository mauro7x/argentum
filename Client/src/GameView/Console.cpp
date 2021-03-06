#include "../../includes/GameView/Console.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Console::_setInputPos() {
    // Centramos solo verticalmente
    input_pos.x = input_box.x;
    input_pos.y = input_box.y + (input_box.h - input.getHeight()) / 2;

    // Scroll horizontal
    int excess = input.getWidth() - input_box.w;
    if (excess > 0) {
        input_pos.x -= excess;
    }
}

void Console::_discardOldMessages() {
    int offset = output_box.h;
    for (auto it = messages.begin(); it != messages.end();) {
        if (offset < 0) {
            it = messages.erase(it);
        } else {
            offset -= it->getHeight();
            it++;
        }
    }
}

void Console::_resetCursorCooldown() {
    show_cursor = true;
    cursor_cooldown = VIEWS_ITERATIONS_TO_SWITCH_CURSOR;
}

void Console::_switchCursorVisibility() {
    if (show_cursor) {
        show_cursor = false;
    } else {
        show_cursor = true;
    }
}

void Console::_renderInputBox() const {
    if (input_enabled) {
        // Renderizar el input
        SDL_Rect render_quad = {input_pos.x, input_pos.y, input.getWidth(),
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

void Console::_renderOutputBox() const {
    SDL_Rect render_quad;
    int y = output_box.h;

    for (auto& message : messages) {
        y -= message.getHeight();
        render_quad = {output_box.x, y, message.getWidth(),
                       message.getHeight()};

        g_renderer->render(message.getTexture(), &render_quad);
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

    // Output
    output_fontsize = config["components"]["output_box"]["fontsize"];
    output_box.x =
        (int)config["components"]["output_box"]["offset"]["x"] + render_rect.x;
    output_box.y =
        (int)config["components"]["output_box"]["offset"]["y"] + render_rect.y;
    output_box.w = (int)config["components"]["output_box"]["w"];
    output_box.h = (int)config["components"]["output_box"]["h"];

    initialized = true;
}

void Console::loadMedia() {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    // Cargar media
    base.loadFromFile(g_renderer, paths::asset(HUD_CONSOLE_BASE_FP));

    // Cargamos la/s fuente/s a usar
    input_font = TTF_OpenFont(CONSOLE_INPUT_FONT.c_str(), input_fontsize);
    output_font = TTF_OpenFont(CONSOLE_OUTPUT_FONT.c_str(), output_fontsize);
    cursor_font = TTF_OpenFont(CONSOLE_CURSOR_FONT.c_str(), input_fontsize);

    if (!input_font || !cursor_font || !output_font) {
        throw Exception("Console::loadMedia: Error opening TTF_Font/s.");
    }

    // Cargamos el input_text vacío
    input.loadFromRenderedText(g_renderer, input_font, " ");
    _setInputPos();

    // Cargamos el cursor
    cursor.loadFromRenderedText(g_renderer, cursor_font, "|");
}

void Console::enableInput() {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    SDL_StartTextInput();
    _resetCursorCooldown();
    input_enabled = true;
}

void Console::append(const char* text) {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    if (current_input.size() < INPUT_MAX_SIZE) {
        current_input += text;
        input_changed = true;
    }
}

void Console::add(const std::string& message, const Color& color) {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    if (!message.empty()) {
        messages.emplace_front();
        messages.front().loadFromRenderedWrappedText(
            g_renderer, output_font, message, output_box.w,
            SDL_Color({color.r, color.g, color.b, color.a}));
        _discardOldMessages();
    }
}

void Console::removeChar() {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    if (!current_input.empty()) {
        current_input.pop_back();
    }

    input_changed = true;
}

std::string Console::getInputText() {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    return current_input;
}

void Console::clearInput() {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    current_input.clear();
    input_changed = true;
}

void Console::disableInput() {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    SDL_StopTextInput();
    show_cursor = false;
    input_enabled = false;
}

void Console::update(const int it) {
    if (!initialized) {
        throw Exception("Console not initialized.");
    }

    if (input_enabled) {
        if (input_changed) {
            if (!current_input.empty()) {
                input.loadFromRenderedText(g_renderer, input_font,
                                           current_input.c_str());
            } else {
                input.loadFromRenderedText(g_renderer, input_font, " ");
            }
            _setInputPos();

            _resetCursorCooldown();
            input_changed = false;
        } else {
            cursor_cooldown -= it;
            while (cursor_cooldown <= 0) {
                _switchCursorVisibility();
                cursor_cooldown += VIEWS_ITERATIONS_TO_SWITCH_CURSOR;
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

    // Renderizamos las boxes
    _renderInputBox();
    _renderOutputBox();
}

Console::~Console() {
    if (input_font) {
        TTF_CloseFont(input_font);
        input_font = NULL;
    }

    if (output_font) {
        TTF_CloseFont(output_font);
        output_font = NULL;
    }

    if (cursor_font) {
        TTF_CloseFont(cursor_font);
        cursor_font = NULL;
    }
}

//-----------------------------------------------------------------------------
