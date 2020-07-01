#include "../includes/ConnectionView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void ConnectionView::_init() {
    json config = JSON::loadJsonFile(CONFIG_FILEPATH);

    // Fuente
    input_fontsize = config["connectionview"]["fontsize"];
    info_fontsize = config["connectionview"]["info"]["fontsize"];

    // TextBoxes
    {
        username_txtbx.render_box.x =
            config["connectionview"]["username"]["offset"]["x"];
        username_txtbx.render_box.y =
            config["connectionview"]["username"]["offset"]["y"];
        username_txtbx.render_box.w = config["connectionview"]["username"]["w"];
        username_txtbx.render_box.h = config["connectionview"]["username"]["h"];

        password_txtbx.render_box.x =
            config["connectionview"]["password"]["offset"]["x"];
        password_txtbx.render_box.y =
            config["connectionview"]["password"]["offset"]["y"];
        password_txtbx.render_box.w = config["connectionview"]["password"]["w"];
        password_txtbx.render_box.h = config["connectionview"]["password"]["h"];
    }

    // Botones
    {
        signup_btn.render_box.x =
            config["connectionview"]["signup"]["offset"]["x"];
        signup_btn.render_box.y =
            config["connectionview"]["signup"]["offset"]["y"];
        signup_btn.render_box.w = config["connectionview"]["signup"]["w"];
        signup_btn.render_box.h = config["connectionview"]["signup"]["h"];

        play_btn.render_box.x = config["connectionview"]["play"]["offset"]["x"];
        play_btn.render_box.y = config["connectionview"]["play"]["offset"]["y"];
        play_btn.render_box.w = config["connectionview"]["play"]["w"];
        play_btn.render_box.h = config["connectionview"]["play"]["h"];
    }

    // Mensajes
    {
        info_box.x = config["connectionview"]["info"]["offset"]["x"];
        info_box.y = config["connectionview"]["info"]["offset"]["y"];
        info_box.w = config["connectionview"]["info"]["w"];
    }
}

void ConnectionView::_loadMedia() {
    // Cargamos el background
    bg.loadFromFile(&renderer, CONNECTIONVIEW_BG_FP);

    // Cargamos las texturas de los text-boxes
    {
        username_txtbx.base.loadFromFile(&renderer, CONNECTIONVIEW_TEXTBOX_FP);
        password_txtbx.base.loadFromFile(&renderer, CONNECTIONVIEW_TEXTBOX_FP);

        username_txtbx.active.loadFromFile(&renderer,
                                           CONNECTIONVIEW_TEXTBOX_ACTIVE_FP);
        password_txtbx.active.loadFromFile(&renderer,
                                           CONNECTIONVIEW_TEXTBOX_ACTIVE_FP);
    }

    // Cargamos las texturas de los botones
    {
        play_btn.base.loadFromFile(&renderer, CONNECTIONVIEW_PLAY_BUTTON_FP);
        play_btn.pressed.loadFromFile(&renderer,
                                      CONNECTIONVIEW_PLAY_BUTTON_PRESSED_FP);
        signup_btn.base.loadFromFile(&renderer,
                                     CONNECTIONVIEW_SIGNUP_BUTTON_FP);
        signup_btn.pressed.loadFromFile(
            &renderer, CONNECTIONVIEW_SIGNUP_BUTTON_PRESSED_FP);
    }

    // Cargamos la/s fuente/s a usar
    input_font = TTF_OpenFont(VIEWS_INPUT_FONT, input_fontsize);
    cursor_font = TTF_OpenFont(VIEWS_CURSOR_FONT, input_fontsize);
    info_font = TTF_OpenFont(VIEWS_INFO_FONT, info_fontsize);

    if (!input_font || !cursor_font || !info_font) {
        throw Exception("ConnectionView::loadMedia: Error opening TTF_Font/s.");
    }

    // Cargamos los input_text vacíos
    username_txtbx.content.loadFromRenderedText(&renderer, input_font, " ",
                                                SDL_Color(VIEWS_FONT_COLOR));
    password_txtbx.content.loadFromRenderedText(&renderer, input_font, " ",
                                                SDL_Color(VIEWS_FONT_COLOR));
    _setInputPos();

    // Cargamos el cursor
    cursor.loadFromRenderedText(&renderer, cursor_font, "|",
                                SDL_Color(VIEWS_FONT_COLOR));

    // Cargamos vacío el msg de información
    info_msg.loadFromRenderedText(&renderer, info_font, " ");
    _setInfoPos();
}

void ConnectionView::_processSDLEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        _handleEvent(e);

        // Propagamos el evento al mixer
        Mixer::handleEvent(e);
    }
}

void ConnectionView::_render() const {
    // Renderizamos el background
    SDL_Rect render_quad = {0, 0, bg.getWidth(), bg.getHeight()};
    renderer.render(bg.getTexture(), &render_quad);

    // Renderizamos el username
    render_quad = username_txtbx.render_box;
    if (username_txtbx.is_active) {
        renderer.render(username_txtbx.active.getTexture(), &render_quad);
    } else {
        renderer.render(username_txtbx.base.getTexture(), &render_quad);
    }

    render_quad = {username_txtbx.render_pos.x, username_txtbx.render_pos.y,
                   username_txtbx.content.getWidth(),
                   username_txtbx.content.getHeight()};
    renderer.render(username_txtbx.content.getTexture(), &render_quad);

    // Renderizamos la contraseña
    render_quad = password_txtbx.render_box;
    if (password_txtbx.is_active) {
        renderer.render(password_txtbx.active.getTexture(), &render_quad);
    } else {
        renderer.render(password_txtbx.base.getTexture(), &render_quad);
    }

    render_quad = {password_txtbx.render_pos.x, password_txtbx.render_pos.y,
                   password_txtbx.content.getWidth(),
                   password_txtbx.content.getHeight()};
    renderer.render(password_txtbx.content.getTexture(), &render_quad);

    // Renderizamos el boton de jugar
    render_quad = play_btn.render_box;
    if (play_btn.mouse_over) {
        renderer.render(play_btn.pressed.getTexture(), &render_quad);
    } else {
        renderer.render(play_btn.base.getTexture(), &render_quad);
    }

    // Renderizamos el boton de crear personaje
    render_quad = signup_btn.render_box;
    if (signup_btn.mouse_over) {
        renderer.render(signup_btn.pressed.getTexture(), &render_quad);
    } else {
        renderer.render(signup_btn.base.getTexture(), &render_quad);
    }

    // Renderizamos el mensaje de información
    render_quad = {info_pos.x, info_pos.y, info_msg.getWidth(),
                   info_msg.getHeight()};
    renderer.render(info_msg.getTexture(), &render_quad);

    // Renderizamos el cursor
    if (show_cursor) {
        if (username_txtbx.is_active) {
            if (!username_txtbx.s_content.empty()) {
                render_quad = {username_txtbx.render_pos.x +
                                   username_txtbx.content.getWidth(),
                               username_txtbx.render_pos.y, cursor.getWidth(),
                               cursor.getHeight()};
            } else {
                render_quad = {username_txtbx.render_pos.x,
                               username_txtbx.render_pos.y, cursor.getWidth(),
                               cursor.getHeight()};
            }

            renderer.render(cursor.getTexture(), &render_quad);
        }

        if (password_txtbx.is_active) {
            if (!password_txtbx.s_content.empty()) {
                render_quad = {password_txtbx.render_pos.x +
                                   password_txtbx.content.getWidth(),
                               password_txtbx.render_pos.y, cursor.getWidth(),
                               cursor.getHeight()};
            } else {
                render_quad = {password_txtbx.render_pos.x,
                               password_txtbx.render_pos.y, cursor.getWidth(),
                               cursor.getHeight()};
            }

            renderer.render(cursor.getTexture(), &render_quad);
        }
    }
}

void ConnectionView::_func(const int it) {
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

void ConnectionView::_handleEvent(const SDL_Event& e) {
    switch (e.type) {
        case SDL_QUIT: {
            current_context = EXIT_CTX;
            quit();
            break;
        }

        case SDL_KEYDOWN: {
            if (e.key.keysym.sym == SDLK_BACKSPACE) {
                if (username_txtbx.is_active &&
                    !username_txtbx.s_content.empty()) {
                    username_txtbx.s_content.pop_back();

                    info_msg.loadFromRenderedText(&renderer, info_font, " ");
                    _setInfoPos();

                    if (!username_txtbx.s_content.empty()) {
                        username_txtbx.content.loadFromRenderedText(
                            &renderer, input_font, username_txtbx.s_content,
                            SDL_Color(VIEWS_FONT_COLOR));
                    } else {
                        username_txtbx.content.loadFromRenderedText(
                            &renderer, input_font, " ",
                            SDL_Color(VIEWS_FONT_COLOR));
                    }

                    break;
                }

                if (password_txtbx.is_active &&
                    !password_txtbx.s_content.empty()) {
                    password_txtbx.s_content.pop_back();

                    info_msg.loadFromRenderedText(&renderer, info_font, " ");
                    _setInfoPos();

                    if (!password_txtbx.s_content.empty()) {
                        password_txtbx.content.loadFromRenderedText(
                            &renderer, input_font, password_txtbx.s_content,
                            SDL_Color(VIEWS_FONT_COLOR));
                    } else {
                        password_txtbx.content.loadFromRenderedText(
                            &renderer, input_font, " ",
                            SDL_Color(VIEWS_FONT_COLOR));
                    }

                    break;
                }

                break;
            }

            if (e.key.keysym.sym == SDLK_RETURN) {
                _handlePlayButtonPressed();
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

                if (username_txtbx.is_active) {
                    username_txtbx.is_active = false;
                    password_txtbx.is_active = true;
                    _resetCursorCooldown();
                } else if (password_txtbx.is_active) {
                    password_txtbx.is_active = false;
                } else {
                    username_txtbx.is_active = true;
                    _resetCursorCooldown();
                }
            }

            break;
        }

        case SDL_TEXTINPUT: {
            if (username_txtbx.is_active) {
                if (username_txtbx.s_content.size() < VIEWS_MAX_INPUT_SIZE) {
                    username_txtbx.s_content += e.text.text;

                    // Re-renderizamos
                    username_txtbx.content.loadFromRenderedText(
                        &renderer, input_font, username_txtbx.s_content,
                        SDL_Color(VIEWS_FONT_COLOR));
                    _setInputPos();

                    // Activamos el cursor
                    _resetCursorCooldown();
                } else {
                    info_msg.loadFromRenderedText(
                        &renderer, info_font, CONNECTIONVIEW_MAX_INPUT_MSG,
                        SDL_Color(VIEWS_WARNING_COLOR));
                    _setInfoPos();
                }

                break;
            }

            if (password_txtbx.is_active) {
                if (password_txtbx.s_content.size() < VIEWS_MAX_INPUT_SIZE) {
                    password_txtbx.s_content += e.text.text;

                    // Re-renderizamos
                    password_txtbx.content.loadFromRenderedText(
                        &renderer, input_font, password_txtbx.s_content,
                        SDL_Color(VIEWS_FONT_COLOR));
                    _setInputPos();

                    // Activamos el cursor
                    _resetCursorCooldown();
                } else {
                    info_msg.loadFromRenderedText(
                        &renderer, info_font, CONNECTIONVIEW_MAX_INPUT_MSG,
                        SDL_Color(VIEWS_WARNING_COLOR));
                    _setInfoPos();
                }

                break;
            }

            break;
        }

        case SDL_MOUSEMOTION: {
            SDL_Point mouse_pos = _getMousePos(e);
            if (_inside(mouse_pos, play_btn.render_box)) {
                play_btn.mouse_over = true;
            } else {
                play_btn.mouse_over = false;
            }

            if (_inside(mouse_pos, signup_btn.render_box)) {
                signup_btn.mouse_over = true;
            } else {
                signup_btn.mouse_over = false;
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

            // Click en el textbox del username
            if (_inside(mouse_pos, username_txtbx.render_box)) {
                username_txtbx.is_active = true;
                _resetCursorCooldown();
            } else {
                username_txtbx.is_active = false;
            }

            // Click en el textbox del password
            if (_inside(mouse_pos, password_txtbx.render_box)) {
                password_txtbx.is_active = true;
                _resetCursorCooldown();
            } else {
                password_txtbx.is_active = false;
            }

            // Click en el botón de jugar
            if (_inside(mouse_pos, play_btn.render_box)) {
                _handlePlayButtonPressed();
                break;
            }

            // Click en el botón de crear cuenta
            if (_inside(mouse_pos, signup_btn.render_box)) {
                _handleSignUpButtonPressed();
                break;
            }

            break;
        }

        default: {
            break;
        }
    }
}

void ConnectionView::_handlePlayButtonPressed() {
    if (username_txtbx.s_content.empty() || password_txtbx.s_content.empty()) {
        info_msg.loadFromRenderedText(&renderer, info_font,
                                      CONNECTIONVIEW_INVALID_INPUT_MSG,
                                      SDL_Color(VIEWS_ERROR_COLOR));
        _setInfoPos();
        return;
    }

    info_msg.loadFromRenderedText(&renderer, info_font,
                                  CONNECTIONVIEW_CONNECTING_MSG,
                                  SDL_Color(VIEWS_FONT_COLOR));
    _setInfoPos();

    // Intentamos conectaros con el server
    _tryToConnect();
}

void ConnectionView::_handleSignUpButtonPressed() {
    current_context = SIGNUP_CTX;
    quit();
}

void ConnectionView::_tryToConnect() {
    // Enviamos la solicitud de conexión de acuerdo al protocolo
    if (!(socket << (uint8_t)SIGN_IN_OPCODE)) {
        throw Exception("ConnectionView::_tryToConnect: socket was closed.");
    }

    if (!(socket << username_txtbx.s_content)) {
        throw Exception("ConnectionView::_tryToConnect: socket was closed.");
    }

    if (!(socket << password_txtbx.s_content)) {
        throw Exception("ConnectionView::_tryToConnect: socket was closed.");
    }

    // Esperamos la respuesta
    uint8_t received_opcode;

    if (!(socket >> received_opcode)) {
        throw Exception("ConnectionView::_tryToConnect: socket was closed.");
    }

    if (received_opcode != CONNECTION_ACK_OPCODE) {
        throw Exception(
            "ConnectionView::_tryToConnect: unknown opcode received.");
    }

    uint8_t ack_type;

    if (!(socket >> ack_type)) {
        throw Exception("ConnectionView::_tryToConnect: socket was closed.");
    }

    switch (ack_type) {
        case SUCCESS_ACK: {
            current_context = GAME_CTX;
            quit();
            break;
        }

        case ERROR_INVALID_USERNAME_ACK: {
            info_msg.loadFromRenderedText(&renderer, info_font,
                                          CONNECTIONVIEW_INVALID_USERNAME_MSG,
                                          SDL_Color(VIEWS_ERROR_COLOR));
            _setInfoPos();
            break;
        }

        case ERROR_INVALID_PASSWORD_ACK: {
            info_msg.loadFromRenderedText(&renderer, info_font,
                                          CONNECTIONVIEW_INVALID_PASSWORD_MSG,
                                          SDL_Color(VIEWS_ERROR_COLOR));
            _setInfoPos();
            break;
        }

        case ERROR_USERNAME_CONNECTED_ACK: {
            info_msg.loadFromRenderedText(&renderer, info_font,
                                          CONNECTIONVIEW_USERNAME_CONNECTED_MSG,
                                          SDL_Color(VIEWS_ERROR_COLOR));
            _setInfoPos();
            break;
        }

        default: {
            throw Exception(
                "ConnectionView::_tryToConnect: unknown message type "
                "received.");
            break;
        }
    }
}

void ConnectionView::_setInputPos() {
    // Centramos solo verticalmente
    username_txtbx.render_pos.x =
        username_txtbx.render_box.x + VIEWS_INPUT_TEXTBOX_X_OFFSET;
    username_txtbx.render_pos.y =
        username_txtbx.render_box.y +
        (username_txtbx.render_box.h - username_txtbx.content.getHeight()) / 2;

    password_txtbx.render_pos.x =
        password_txtbx.render_box.x + VIEWS_INPUT_TEXTBOX_X_OFFSET;
    password_txtbx.render_pos.y =
        password_txtbx.render_box.y +
        (password_txtbx.render_box.h - password_txtbx.content.getHeight()) / 2;
}

void ConnectionView::_setInfoPos() {
    // Centramos vertical y horizontalmente
    info_pos.x = info_box.x + (info_box.w - info_msg.getWidth()) / 2;
    info_pos.y = info_box.y;
}

SDL_Point ConnectionView::_getMousePos(const SDL_Event& e) const {
    return SDL_Point({((int)(e.button.x / renderer.getWidthScaleFactor())),
                      ((int)(e.button.y / renderer.getHeightScaleFactor()))});
}

bool ConnectionView::_inside(const SDL_Point& pos, const SDL_Rect& box) const {
    if ((pos.x > (box.x + box.w)) || (pos.x < (box.x)) ||
        (pos.y > (box.y + box.h)) || (pos.y < (box.y))) {
        return false;
    }

    return true;
}

void ConnectionView::_updateCursorAnimation(const int it) {
    if (username_txtbx.is_active || password_txtbx.is_active) {
        cursor_cooldown -= it;
        while (cursor_cooldown <= 0) {
            _switchCursorVisibility();
            cursor_cooldown += VIEWS_ITERATIONS_TO_SWITCH_CURSOR;
        }
    }
}

void ConnectionView::_resetCursorCooldown() {
    show_cursor = true;
    cursor_cooldown = VIEWS_ITERATIONS_TO_SWITCH_CURSOR;
}

void ConnectionView::_switchCursorVisibility() {
    if (show_cursor) {
        show_cursor = false;
    } else {
        show_cursor = true;
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ConnectionView::ConnectionView(Context& current_context,
                               const Renderer& renderer,
                               const SocketWrapper& socket)
    : ConstantRateFunc(RATE),
      current_context(current_context),
      renderer(renderer),

      socket(socket) {
    _init();
    _loadMedia();
    SDL_StartTextInput();
}

ConnectionView::~ConnectionView() {
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
