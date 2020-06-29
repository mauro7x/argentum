#include "../includes/ConnectionView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void ConnectionView::_init() {
    json config = JSON::loadJsonFile(CONFIG_FILEPATH);

    // Fuente
    input_fontsize = config["connectionview"]["fontsize"];
    info_fontsize = config["connectionview"]["info"]["fontsize"];

    // Offsets de renderizado
    username_box.x = config["connectionview"]["username"]["offset"]["x"];
    username_box.y = config["connectionview"]["username"]["offset"]["y"];
    username_box.w = config["connectionview"]["username"]["w"];
    username_box.h = config["connectionview"]["username"]["h"];

    password_box.x = config["connectionview"]["password"]["offset"]["x"];
    password_box.y = config["connectionview"]["password"]["offset"]["y"];
    password_box.w = config["connectionview"]["password"]["w"];
    password_box.h = config["connectionview"]["password"]["h"];

    signup_box.x = config["connectionview"]["signup"]["offset"]["x"];
    signup_box.y = config["connectionview"]["signup"]["offset"]["y"];
    signup_box.w = config["connectionview"]["signup"]["w"];
    signup_box.h = config["connectionview"]["signup"]["h"];

    play_box.x = config["connectionview"]["play"]["offset"]["x"];
    play_box.y = config["connectionview"]["play"]["offset"]["y"];
    play_box.w = config["connectionview"]["play"]["w"];
    play_box.h = config["connectionview"]["play"]["h"];

    info_box.x = config["connectionview"]["info"]["offset"]["x"];
    info_box.y = config["connectionview"]["info"]["offset"]["y"];
    info_box.w = config["connectionview"]["info"]["w"];
}

void ConnectionView::_loadMedia() {
    // Cargamos el background
    bg.loadFromFile(&renderer, CONNECTIONVIEW_BG_FP);

    // Cargamos las imagenes estáticas
    input_inactive_box.loadFromFile(&renderer, CONNECTIONVIEW_TEXTBOX_FP);
    input_active_box.loadFromFile(&renderer, CONNECTIONVIEW_TEXTBOX_ACTIVE_FP);

    play_button.loadFromFile(&renderer, CONNECTIONVIEW_PLAY_BUTTON_FP);
    play_button_pressed.loadFromFile(&renderer,
                                     CONNECTIONVIEW_PLAY_BUTTON_PRESSED_FP);
    signup_button.loadFromFile(&renderer, CONNECTIONVIEW_SIGNUP_BUTTON_FP);
    signup_button_pressed.loadFromFile(&renderer,
                                       CONNECTIONVIEW_SIGNUP_BUTTON_PRESSED_FP);

    // Cargamos la/s fuente/s a usar
    input_font = TTF_OpenFont(VIEWS_INPUT_FONT, input_fontsize);
    cursor_font = TTF_OpenFont(VIEWS_CURSOR_FONT, input_fontsize);
    info_font = TTF_OpenFont(VIEWS_INFO_FONT, info_fontsize);

    if (!input_font || !cursor_font || !info_font) {
        throw Exception("ConnectionView::loadMedia: Error opening TTF_Font/s.");
    }

    // Cargamos los input_text vacíos
    username.loadFromRenderedText(&renderer, input_font, " ",
                                  SDL_Color(VIEWS_FONT_COLOR));
    password.loadFromRenderedText(&renderer, input_font, " ",
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
    }
}

void ConnectionView::_render() const {
    // Renderizamos el background
    SDL_Rect render_quad = {0, 0, bg.getWidth(), bg.getHeight()};
    renderer.render(bg.getTexture(), &render_quad);

    // Renderizamos el username
    render_quad = username_box;
    if (username_active) {
        renderer.render(input_active_box.getTexture(), &render_quad);
    } else {
        renderer.render(input_inactive_box.getTexture(), &render_quad);
    }

    render_quad = {username_pos.x, username_pos.y, username.getWidth(),
                   username.getHeight()};
    renderer.render(username.getTexture(), &render_quad);

    // Renderizamos la contraseña
    render_quad = password_box;
    if (password_active) {
        renderer.render(input_active_box.getTexture(), &render_quad);
    } else {
        renderer.render(input_inactive_box.getTexture(), &render_quad);
    }

    render_quad = {password_pos.x, password_pos.y, password.getWidth(),
                   password.getHeight()};
    renderer.render(password.getTexture(), &render_quad);

    // Renderizamos el boton de jugar
    render_quad = play_box;
    if (play_button_over) {
        renderer.render(play_button_pressed.getTexture(), &render_quad);
    } else {
        renderer.render(play_button.getTexture(), &render_quad);
    }

    // Renderizamos el boton de crear personaje
    render_quad = signup_box;
    if (signup_button_over) {
        renderer.render(signup_button_pressed.getTexture(), &render_quad);
    } else {
        renderer.render(signup_button.getTexture(), &render_quad);
    }

    // Renderizamos el mensaje de información
    render_quad = {info_pos.x, info_pos.y, info_msg.getWidth(),
                   info_msg.getHeight()};
    renderer.render(info_msg.getTexture(), &render_quad);

    // Renderizamos el cursor
    if (show_cursor) {
        if (username_active) {
            if (!current_username.empty()) {
                render_quad = {username_pos.x + username.getWidth(),
                               username_pos.y, cursor.getWidth(),
                               cursor.getHeight()};
            } else {
                render_quad = {username_pos.x, username_pos.y,
                               cursor.getWidth(), cursor.getHeight()};
            }

            renderer.render(cursor.getTexture(), &render_quad);
        }

        if (password_active) {
            if (!current_password.empty()) {
                render_quad = {password_pos.x + password.getWidth(),
                               password_pos.y, cursor.getWidth(),
                               cursor.getHeight()};
            } else {
                render_quad = {password_pos.x, password_pos.y,
                               cursor.getWidth(), cursor.getHeight()};
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
                if (username_active && !current_username.empty()) {
                    current_username.pop_back();

                    info_msg.loadFromRenderedText(&renderer, info_font, " ");
                    _setInfoPos();

                    if (!current_username.empty()) {
                        username.loadFromRenderedText(
                            &renderer, input_font, current_username,
                            SDL_Color(VIEWS_FONT_COLOR));
                    } else {
                        username.loadFromRenderedText(
                            &renderer, input_font, " ",
                            SDL_Color(VIEWS_FONT_COLOR));
                    }

                    break;
                }

                if (password_active && !current_password.empty()) {
                    current_password.pop_back();

                    info_msg.loadFromRenderedText(&renderer, info_font, " ");
                    _setInfoPos();

                    if (!current_password.empty()) {
                        password.loadFromRenderedText(
                            &renderer, input_font, current_password,
                            SDL_Color(VIEWS_FONT_COLOR));
                    } else {
                        password.loadFromRenderedText(
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

                if (username_active) {
                    username_active = false;
                    password_active = true;
                    _resetCursorCooldown();
                } else if (password_active) {
                    password_active = false;
                } else {
                    username_active = true;
                    _resetCursorCooldown();
                }
            }

            break;
        }

        case SDL_TEXTINPUT: {
            if (username_active) {
                if (current_username.size() < VIEWS_MAX_INPUT_SIZE) {
                    current_username += e.text.text;

                    // Re-renderizamos
                    username.loadFromRenderedText(&renderer, input_font,
                                                  current_username,
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

            if (password_active) {
                if (current_password.size() < VIEWS_MAX_INPUT_SIZE) {
                    current_password += e.text.text;

                    // Re-renderizamos
                    password.loadFromRenderedText(&renderer, input_font,
                                                  current_password,
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
            if (_inside(mouse_pos, play_box)) {
                play_button_over = true;
            } else {
                play_button_over = false;
            }

            if (_inside(mouse_pos, signup_box)) {
                signup_button_over = true;
            } else {
                signup_button_over = false;
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
            if (_inside(mouse_pos, username_box)) {
                username_active = true;
                _resetCursorCooldown();
            } else {
                username_active = false;
            }

            // Click en el textbox del password
            if (_inside(mouse_pos, password_box)) {
                password_active = true;
                _resetCursorCooldown();
            } else {
                password_active = false;
            }

            // Click en el botón de jugar
            if (_inside(mouse_pos, play_box)) {
                _handlePlayButtonPressed();
                break;
            }

            // Click en el botón de crear cuenta
            if (_inside(mouse_pos, signup_box)) {
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
    if (current_username.empty() || current_password.empty()) {
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

    // INTENTAR CONECTAR CON EL SERVER
    // (mandar msj de login con username y password)
    current_context = GAME_CTX;
    quit();
}

void ConnectionView::_handleSignUpButtonPressed() {
    current_context = SIGNUP_CTX;
    quit();
}

void ConnectionView::_setInputPos() {
    // Centramos solo verticalmente
    username_pos.x = username_box.x + VIEWS_INPUT_TEXTBOX_X_OFFSET;
    username_pos.y =
        username_box.y + (username_box.h - username.getHeight()) / 2;

    password_pos.x = password_box.x + VIEWS_INPUT_TEXTBOX_X_OFFSET;
    password_pos.y =
        password_box.y + (password_box.h - password.getHeight()) / 2;
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
    if (username_active || password_active) {
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

ConnectionView::ConnectionView(Context& current_context, Renderer& renderer,
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
