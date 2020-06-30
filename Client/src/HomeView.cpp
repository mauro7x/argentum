#include "../includes/HomeView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void HomeView::_init() {
    json config = JSON::loadJsonFile(CONFIG_FILEPATH);

    // Fuente
    input_fontsize = config["homeview"]["fontsize"];
    info_fontsize = config["homeview"]["info"]["fontsize"];

    // Offsets de renderizado
    hostname_txtbx.render_box.x = config["homeview"]["hostname"]["offset"]["x"];
    hostname_txtbx.render_box.y = config["homeview"]["hostname"]["offset"]["y"];
    hostname_txtbx.render_box.w = config["homeview"]["hostname"]["w"];
    hostname_txtbx.render_box.h = config["homeview"]["hostname"]["h"];

    port_txtbx.render_box.x = config["homeview"]["port"]["offset"]["x"];
    port_txtbx.render_box.y = config["homeview"]["port"]["offset"]["y"];
    port_txtbx.render_box.w = config["homeview"]["port"]["w"];
    port_txtbx.render_box.h = config["homeview"]["port"]["h"];

    connect_btn.render_box.x = config["homeview"]["connect"]["offset"]["x"];
    connect_btn.render_box.y = config["homeview"]["connect"]["offset"]["y"];
    connect_btn.render_box.w = config["homeview"]["connect"]["w"];
    connect_btn.render_box.h = config["homeview"]["connect"]["h"];

    info_box.x = config["homeview"]["info"]["offset"]["x"];
    info_box.y = config["homeview"]["info"]["offset"]["y"];
    info_box.w = config["homeview"]["info"]["w"];
}

void HomeView::_loadMedia() {
    // Cargamos el background
    bg.loadFromFile(&renderer, HOMEVIEW_BG_FP);

    // Cargamos las imagenes estáticas
    hostname_txtbx.base.loadFromFile(&renderer, HOMEVIEW_TEXTBOX_FP);
    port_txtbx.base.loadFromFile(&renderer, HOMEVIEW_TEXTBOX_FP);

    hostname_txtbx.active.loadFromFile(&renderer, HOMEVIEW_TEXTBOX_ACTIVE_FP);
    port_txtbx.active.loadFromFile(&renderer, HOMEVIEW_TEXTBOX_ACTIVE_FP);

    connect_btn.base.loadFromFile(&renderer, HOMEVIEW_CONNECT_BUTTON_FP);
    connect_btn.pressed.loadFromFile(&renderer,
                                     HOMEVIEW_CONNECT_BUTTON_PRESSED_FP);

    // Cargamos la/s fuente/s a usar
    input_font = TTF_OpenFont(VIEWS_INPUT_FONT, input_fontsize);
    cursor_font = TTF_OpenFont(VIEWS_CURSOR_FONT, input_fontsize);
    info_font = TTF_OpenFont(VIEWS_INFO_FONT, info_fontsize);

    if (!input_font || !cursor_font || !info_font) {
        throw Exception("HomeView::loadMedia: Error opening TTF_Font/s.");
    }

    // Cargamos los input_text vacíos
    hostname_txtbx.content.loadFromRenderedText(&renderer, input_font, " ",
                                                SDL_Color(VIEWS_FONT_COLOR));
    port_txtbx.content.loadFromRenderedText(&renderer, input_font, " ",
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

        // Propagamos el evento al mixer
        mixer.handleEvent(e);
    }
}

void HomeView::_render() const {
    // Renderizamos el background
    SDL_Rect render_quad = {0, 0, bg.getWidth(), bg.getHeight()};
    renderer.render(bg.getTexture(), &render_quad);

    // Renderizamos el hostname
    render_quad = hostname_txtbx.render_box;
    if (hostname_txtbx.is_active) {
        renderer.render(hostname_txtbx.active.getTexture(), &render_quad);
    } else {
        renderer.render(hostname_txtbx.base.getTexture(), &render_quad);
    }

    render_quad = {hostname_txtbx.render_pos.x, hostname_txtbx.render_pos.y,
                   hostname_txtbx.content.getWidth(),
                   hostname_txtbx.content.getHeight()};
    renderer.render(hostname_txtbx.content.getTexture(), &render_quad);

    // Renderizamos el puerto
    render_quad = port_txtbx.render_box;
    if (port_txtbx.is_active) {
        renderer.render(port_txtbx.active.getTexture(), &render_quad);
    } else {
        renderer.render(port_txtbx.base.getTexture(), &render_quad);
    }

    render_quad = {port_txtbx.render_pos.x, port_txtbx.render_pos.y,
                   port_txtbx.content.getWidth(),
                   port_txtbx.content.getHeight()};
    renderer.render(port_txtbx.content.getTexture(), &render_quad);

    // Renderizamos el boton de conectar
    render_quad = connect_btn.render_box;
    if (connect_btn.mouse_over) {
        renderer.render(connect_btn.pressed.getTexture(), &render_quad);
    } else {
        renderer.render(connect_btn.base.getTexture(), &render_quad);
    }

    // Renderizamos el mensaje de información
    render_quad = {info_pos.x, info_pos.y, info_msg.getWidth(),
                   info_msg.getHeight()};
    renderer.render(info_msg.getTexture(), &render_quad);

    // Renderizamos el cursor
    if (show_cursor) {
        if (hostname_txtbx.is_active) {
            if (!hostname_txtbx.s_content.empty()) {
                render_quad = {hostname_txtbx.render_pos.x +
                                   hostname_txtbx.content.getWidth(),
                               hostname_txtbx.render_pos.y, cursor.getWidth(),
                               cursor.getHeight()};
            } else {
                render_quad = {hostname_txtbx.render_pos.x,
                               hostname_txtbx.render_pos.y, cursor.getWidth(),
                               cursor.getHeight()};
            }

            renderer.render(cursor.getTexture(), &render_quad);
        }

        if (port_txtbx.is_active) {
            if (!port_txtbx.s_content.empty()) {
                render_quad = {
                    port_txtbx.render_pos.x + port_txtbx.content.getWidth(),
                    port_txtbx.render_pos.y, cursor.getWidth(),
                    cursor.getHeight()};
            } else {
                render_quad = {port_txtbx.render_pos.x, port_txtbx.render_pos.y,
                               cursor.getWidth(), cursor.getHeight()};
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
                if (hostname_txtbx.is_active &&
                    !hostname_txtbx.s_content.empty()) {
                    hostname_txtbx.s_content.pop_back();

                    info_msg.loadFromRenderedText(&renderer, info_font, " ");
                    _setInfoPos();

                    if (!hostname_txtbx.s_content.empty()) {
                        hostname_txtbx.content.loadFromRenderedText(
                            &renderer, input_font, hostname_txtbx.s_content,
                            SDL_Color(VIEWS_FONT_COLOR));
                    } else {
                        hostname_txtbx.content.loadFromRenderedText(
                            &renderer, input_font, " ",
                            SDL_Color(VIEWS_FONT_COLOR));
                    }

                    break;
                }

                if (port_txtbx.is_active && !port_txtbx.s_content.empty()) {
                    port_txtbx.s_content.pop_back();

                    info_msg.loadFromRenderedText(&renderer, info_font, " ");
                    _setInfoPos();

                    if (!port_txtbx.s_content.empty()) {
                        port_txtbx.content.loadFromRenderedText(
                            &renderer, input_font, port_txtbx.s_content,
                            SDL_Color(VIEWS_FONT_COLOR));
                    } else {
                        port_txtbx.content.loadFromRenderedText(
                            &renderer, input_font, " ",
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

                if (hostname_txtbx.is_active) {
                    hostname_txtbx.is_active = false;
                    port_txtbx.is_active = true;
                    _resetCursorCooldown();
                } else if (port_txtbx.is_active) {
                    port_txtbx.is_active = false;
                } else {
                    hostname_txtbx.is_active = true;
                    _resetCursorCooldown();
                }
            }

            break;
        }

        case SDL_TEXTINPUT: {
            if (hostname_txtbx.is_active) {
                if (hostname_txtbx.s_content.size() < VIEWS_MAX_INPUT_SIZE) {
                    hostname_txtbx.s_content += e.text.text;

                    // Re-renderizamos
                    hostname_txtbx.content.loadFromRenderedText(
                        &renderer, input_font, hostname_txtbx.s_content,
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

            if (port_txtbx.is_active) {
                if (port_txtbx.s_content.size() < VIEWS_MAX_INPUT_SIZE) {
                    port_txtbx.s_content += e.text.text;

                    // Re-renderizamos
                    port_txtbx.content.loadFromRenderedText(
                        &renderer, input_font, port_txtbx.s_content,
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
            if (_inside(mouse_pos, connect_btn.render_box)) {
                connect_btn.mouse_over = true;
            } else {
                connect_btn.mouse_over = false;
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
            if (_inside(mouse_pos, hostname_txtbx.render_box)) {
                hostname_txtbx.is_active = true;
                _resetCursorCooldown();
            } else {
                hostname_txtbx.is_active = false;
            }

            // Click en el textbox del port
            if (_inside(mouse_pos, port_txtbx.render_box)) {
                port_txtbx.is_active = true;
                _resetCursorCooldown();
            } else {
                port_txtbx.is_active = false;
            }

            // Click en el botón de conectar
            if (_inside(mouse_pos, connect_btn.render_box)) {
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
    if (hostname_txtbx.s_content.empty() || port_txtbx.s_content.empty()) {
        info_msg.loadFromRenderedText(&renderer, info_font,
                                      HOMEVIEW_INVALID_INPUT_MSG,
                                      SDL_Color(VIEWS_ERROR_COLOR));
        _setInfoPos();
        return;
    }

    info_msg.loadFromRenderedText(&renderer, info_font, HOMEVIEW_CONNECTING_MSG,
                                  SDL_Color(VIEWS_FONT_COLOR));
    _setInfoPos();

    // intentar conexión
    try {
        socket = std::move(
            SocketWrapper(hostname_txtbx.s_content, port_txtbx.s_content));
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
    hostname_txtbx.render_pos.x =
        hostname_txtbx.render_box.x + VIEWS_INPUT_TEXTBOX_X_OFFSET;
    hostname_txtbx.render_pos.y =
        hostname_txtbx.render_box.y +
        (hostname_txtbx.render_box.h - hostname_txtbx.content.getHeight()) / 2;

    port_txtbx.render_pos.x =
        port_txtbx.render_box.x + VIEWS_INPUT_TEXTBOX_X_OFFSET;
    port_txtbx.render_pos.y =
        port_txtbx.render_box.y +
        (port_txtbx.render_box.h - port_txtbx.content.getHeight()) / 2;
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
    if (hostname_txtbx.is_active || port_txtbx.is_active) {
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

HomeView::HomeView(Context& current_context, const Renderer& renderer,
                   Mixer& mixer, SocketWrapper& socket)
    : ConstantRateFunc(RATE),
      current_context(current_context),
      renderer(renderer),
      mixer(mixer),
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
