#include "../includes/HomeView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void HomeView::_init() {
    json config = JSON::loadJsonFile(CONFIG_FILEPATH);

    // Fuente
    input_fontsize = config["homeview"]["fontsize"];
    info_fontsize = config["homeview"]["info"]["fontsize"];

    // Offsets de renderizado
    hostname_box.x = config["homeview"]["hostname"]["offset"]["x"];
    hostname_box.y = config["homeview"]["hostname"]["offset"]["y"];
    hostname_box.w = config["homeview"]["hostname"]["w"];
    hostname_box.h = config["homeview"]["hostname"]["h"];

    port_box.x = config["homeview"]["port"]["offset"]["x"];
    port_box.y = config["homeview"]["port"]["offset"]["y"];
    port_box.w = config["homeview"]["port"]["w"];
    port_box.h = config["homeview"]["port"]["h"];

    connect_box.x = config["homeview"]["connect"]["offset"]["x"];
    connect_box.y = config["homeview"]["connect"]["offset"]["y"];
    connect_box.w = config["homeview"]["connect"]["w"];
    connect_box.h = config["homeview"]["connect"]["h"];

    info_box.x = config["homeview"]["info"]["offset"]["x"];
    info_box.y = config["homeview"]["info"]["offset"]["y"];
    info_box.w = config["homeview"]["info"]["w"];
}

void HomeView::_loadMedia() {
    // Cargamos el background
    bg.loadFromFile(&renderer, HOMEVIEW_BG_FP);

    // Cargamos las imagenes estáticas
    hostname_inactive_box.loadFromFile(&renderer, HOMEVIEW_TEXTBOX_FP);
    hostname_active_box.loadFromFile(&renderer, HOMEVIEW_TEXTBOX_ACTIVE_FP);
    port_inactive_box.loadFromFile(&renderer, HOMEVIEW_TEXTBOX_FP);
    port_active_box.loadFromFile(&renderer, HOMEVIEW_TEXTBOX_ACTIVE_FP);
    connect_button.loadFromFile(&renderer, HOMEVIEW_CONNECT_BUTTON_FP);
    connect_button_pressed.loadFromFile(&renderer,
                                        HOMEVIEW_CONNECT_BUTTON_PRESSED_FP);

    // Cargamos la/s fuente/s a usar
    input_font = TTF_OpenFont(VIEWS_INPUT_FONT, input_fontsize);
    cursor_font = TTF_OpenFont(VIEWS_CURSOR_FONT, input_fontsize);
    info_font = TTF_OpenFont(VIEWS_INFO_FONT, info_fontsize);

    if (!input_font || !cursor_font || !info_font) {
        throw Exception("HomeView::loadMedia: Error opening TTF_Font/s.");
    }

    // Cargamos los input_text vacíos
    hostname.loadFromRenderedText(&renderer, input_font, " ",
                                  SDL_Color(VIEWS_FONT_COLOR));
    port.loadFromRenderedText(&renderer, input_font, " ",
                              SDL_Color(VIEWS_FONT_COLOR));
    _setInputPos();

    // Cargamos el cursor
    cursor.loadFromRenderedText(&renderer, cursor_font, "|",
                                SDL_Color(VIEWS_FONT_COLOR));

    // Cargamos vacío el msg de información
    info_msg.loadFromRenderedText(&renderer, info_font, " ");
    _setInfoPos();
}

void HomeView::_processSDLEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        _handleEvent(e);
    }
}

void HomeView::_render() const {
    // Renderizamos el background
    SDL_Rect render_quad = {0, 0, bg.getWidth(), bg.getHeight()};
    renderer.render(bg.getTexture(), &render_quad);

    // Renderizamos el hostname
    render_quad = hostname_box;
    if (hostname_active) {
        renderer.render(hostname_active_box.getTexture(), &render_quad);
    } else {
        renderer.render(hostname_inactive_box.getTexture(), &render_quad);
    }

    render_quad = {hostname_pos.x, hostname_pos.y, hostname.getWidth(),
                   hostname.getHeight()};
    renderer.render(hostname.getTexture(), &render_quad);

    // Renderizamos el puerto
    render_quad = port_box;
    if (port_active) {
        renderer.render(port_active_box.getTexture(), &render_quad);
    } else {
        renderer.render(port_inactive_box.getTexture(), &render_quad);
    }

    render_quad = {port_pos.x, port_pos.y, port.getWidth(), port.getHeight()};
    renderer.render(port.getTexture(), &render_quad);

    // Renderizamos el boton de conectar
    render_quad = connect_box;
    if (connect_button_over) {
        renderer.render(connect_button_pressed.getTexture(), &render_quad);
    } else {
        renderer.render(connect_button.getTexture(), &render_quad);
    }

    // Renderizamos el mensaje de información
    render_quad = {info_pos.x, info_pos.y, info_msg.getWidth(),
                   info_msg.getHeight()};
    renderer.render(info_msg.getTexture(), &render_quad);

    // Renderizamos el cursor
    if (show_cursor) {
        if (hostname_active) {
            if (!current_hostname.empty()) {
                render_quad = {hostname_pos.x + hostname.getWidth(),
                               hostname_pos.y, cursor.getWidth(),
                               cursor.getHeight()};
            } else {
                render_quad = {hostname_pos.x, hostname_pos.y,
                               cursor.getWidth(), cursor.getHeight()};
            }

            renderer.render(cursor.getTexture(), &render_quad);
        }

        if (port_active) {
            if (!current_port.empty()) {
                render_quad = {port_pos.x + port.getWidth(), port_pos.y,
                               cursor.getWidth(), cursor.getHeight()};
            } else {
                render_quad = {port_pos.x, port_pos.y, cursor.getWidth(),
                               cursor.getHeight()};
            }

            renderer.render(cursor.getTexture(), &render_quad);
        }
    }
}

void HomeView::_func(const int it) {
    /* Vaciamos las colas a procesar*/
    _processSDLEvents();

    /* Limpiamos la pantalla */
    renderer.clearScreen();

    /* Renderizamos y presentamos la pantalla */
    _render();

    /* Actualizamos el cooldown del cursor */
    _updateCursorAnimation(it);

    /* Presentamos la pantalla */
    renderer.presentScreen();
}

//-----------------------------------------------------------------------------
// Auxiliares

void HomeView::_handleEvent(const SDL_Event& e) {
    switch (e.type) {
        case SDL_QUIT: {
            current_context = EXIT_CTX;
            quit();
            break;
        }

        case SDL_KEYDOWN: {
            if (e.key.keysym.sym == SDLK_BACKSPACE) {
                if (hostname_active && !current_hostname.empty()) {
                    current_hostname.pop_back();

                    info_msg.loadFromRenderedText(&renderer, info_font, " ");
                    _setInfoPos();

                    if (!current_hostname.empty()) {
                        hostname.loadFromRenderedText(
                            &renderer, input_font, current_hostname,
                            SDL_Color(VIEWS_FONT_COLOR));
                    } else {
                        hostname.loadFromRenderedText(
                            &renderer, input_font, " ",
                            SDL_Color(VIEWS_FONT_COLOR));
                    }

                    break;
                }

                if (port_active && !current_port.empty()) {
                    current_port.pop_back();

                    info_msg.loadFromRenderedText(&renderer, info_font, " ");
                    _setInfoPos();

                    if (!current_port.empty()) {
                        port.loadFromRenderedText(&renderer, input_font,
                                                  current_port,
                                                  SDL_Color(VIEWS_FONT_COLOR));
                    } else {
                        port.loadFromRenderedText(&renderer, input_font, " ",
                                                  SDL_Color(VIEWS_FONT_COLOR));
                    }

                    break;
                }

                break;
            }

            if (e.key.keysym.sym == SDLK_RETURN) {
                _handleConnectButtonPressed();
                break;
            }

            if (e.key.keysym.sym == SDLK_ESCAPE) {
                current_context = EXIT_CTX;
                quit();
                break;
            }

            if (e.key.keysym.sym == SDLK_TAB) {
                info_msg.loadFromRenderedText(&renderer, info_font, " ");
                _setInfoPos();

                if (hostname_active) {
                    hostname_active = false;
                    port_active = true;
                    _resetCursorCooldown();
                } else if (port_active) {
                    port_active = false;
                } else {
                    hostname_active = true;
                    _resetCursorCooldown();
                }
            }

            break;
        }

        case SDL_TEXTINPUT: {
            if (hostname_active) {
                if (current_hostname.size() < VIEWS_MAX_INPUT_SIZE) {
                    current_hostname += e.text.text;

                    // Re-renderizamos
                    hostname.loadFromRenderedText(&renderer, input_font,
                                                  current_hostname,
                                                  SDL_Color(VIEWS_FONT_COLOR));
                    _setInputPos();

                    // Activamos el cursor
                    _resetCursorCooldown();
                } else {
                    info_msg.loadFromRenderedText(
                        &renderer, info_font, HOMEVIEW_MAX_INPUT_MSG,
                        SDL_Color(VIEWS_WARNING_COLOR));
                    _setInfoPos();
                }

                break;
            }

            if (port_active) {
                if (current_port.size() < VIEWS_MAX_INPUT_SIZE) {
                    current_port += e.text.text;

                    // Re-renderizamos
                    port.loadFromRenderedText(&renderer, input_font,
                                              current_port,
                                              SDL_Color(VIEWS_FONT_COLOR));
                    _setInputPos();

                    // Activamos el cursor
                    _resetCursorCooldown();
                } else {
                    info_msg.loadFromRenderedText(
                        &renderer, info_font, HOMEVIEW_MAX_INPUT_MSG,
                        SDL_Color(VIEWS_WARNING_COLOR));
                    _setInfoPos();
                }

                break;
            }

            break;
        }

        case SDL_MOUSEMOTION: {
            SDL_Point mouse_pos = _getMousePos(e);
            if (_inside(mouse_pos, connect_box)) {
                connect_button_over = true;
            } else {
                connect_button_over = false;
            }

            break;
        }

        case SDL_MOUSEBUTTONDOWN: {
            if (e.button.button != SDL_BUTTON_LEFT) {
                break;
            }

            info_msg.loadFromRenderedText(&renderer, info_font, " ");
            _setInfoPos();

            SDL_Point mouse_pos = _getMousePos(e);
            show_cursor = false;

            // Click en el textbox del hostname
            if (_inside(mouse_pos, hostname_box)) {
                hostname_active = true;
                _resetCursorCooldown();
            } else {
                hostname_active = false;
            }

            // Click en el textbox del port
            if (_inside(mouse_pos, port_box)) {
                port_active = true;
                _resetCursorCooldown();
            } else {
                port_active = false;
            }

            // Click en el botón de conectar
            if (_inside(mouse_pos, connect_box)) {
                _handleConnectButtonPressed();
                break;
            }

            break;
        }

        default: {
            break;
        }
    }
}

void HomeView::_handleConnectButtonPressed() {
    if (current_hostname.empty() || current_port.empty()) {
        info_msg.loadFromRenderedText(&renderer, info_font,
                                      HOMEVIEW_INVALID_INPUT_MSG,
                                      SDL_Color(VIEWS_WARNING_COLOR));
        _setInfoPos();
        return;
    }

    info_msg.loadFromRenderedText(&renderer, info_font, HOMEVIEW_CONNECTING_MSG,
                                  SDL_Color(VIEWS_FONT_COLOR));
    _setInfoPos();

    // intentar conexión
    try {
        socket = std::move(SocketWrapper(current_hostname, current_port));
    } catch (const Exception& e) {
        info_msg.loadFromRenderedText(&renderer, info_font,
                                      HOMEVIEW_ERROR_CONNECTING_MSG,
                                      SDL_Color(VIEWS_ERROR_COLOR));
        _setInfoPos();
        return;
    }

    current_context = CONNECTION_CTX;
    quit();
}

void HomeView::_setInputPos() {
    // Centramos solo verticalmente
    hostname_pos.x = hostname_box.x + VIEWS_INPUT_TEXTBOX_X_OFFSET;
    hostname_pos.y =
        hostname_box.y + (hostname_box.h - hostname.getHeight()) / 2;

    port_pos.x = port_box.x + VIEWS_INPUT_TEXTBOX_X_OFFSET;
    port_pos.y = port_box.y + (port_box.h - port.getHeight()) / 2;
}

void HomeView::_setInfoPos() {
    // Centramos vertical y horizontalmente
    info_pos.x = info_box.x + (info_box.w - info_msg.getWidth()) / 2;
    info_pos.y = info_box.y;
}

SDL_Point HomeView::_getMousePos(const SDL_Event& e) const {
    return SDL_Point({((int)(e.button.x / renderer.getWidthScaleFactor())),
                      ((int)(e.button.y / renderer.getHeightScaleFactor()))});
}

bool HomeView::_inside(const SDL_Point& pos, const SDL_Rect& box) const {
    if ((pos.x > (box.x + box.w)) || (pos.x < (box.x)) ||
        (pos.y > (box.y + box.h)) || (pos.y < (box.y))) {
        return false;
    }

    return true;
}

void HomeView::_updateCursorAnimation(const int it) {
    if (hostname_active || port_active) {
        cursor_cooldown -= it;
        while (cursor_cooldown <= 0) {
            _switchCursorVisibility();
            cursor_cooldown += VIEWS_ITERATIONS_TO_SWITCH_CURSOR;
        }
    }
}

void HomeView::_resetCursorCooldown() {
    show_cursor = true;
    cursor_cooldown = VIEWS_ITERATIONS_TO_SWITCH_CURSOR;
}

void HomeView::_switchCursorVisibility() {
    if (show_cursor) {
        show_cursor = false;
    } else {
        show_cursor = true;
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

HomeView::HomeView(Context& current_context, Renderer& renderer,
                   SocketWrapper& socket)
    : ConstantRateFunc(RATE),
      current_context(current_context),
      renderer(renderer),
      socket(socket) {
    _init();
    _loadMedia();
    SDL_StartTextInput();
}

HomeView::~HomeView() {
    if (input_font) {
        TTF_CloseFont(input_font);
        input_font = NULL;
    }

    if (cursor_font) {
        TTF_CloseFont(cursor_font);
        cursor_font = NULL;
    }

    if (info_font) {
        TTF_CloseFont(info_font);
        info_font = NULL;
    }

    SDL_StopTextInput();
}

//-----------------------------------------------------------------------------
