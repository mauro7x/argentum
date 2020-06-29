#include "../includes/SignUpView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void SignUpView::_init() {
    json config = JSON::loadJsonFile(CONFIG_FILEPATH);

    // Fuente
    input_fontsize = config["signupview"]["fontsize"];
    info_fontsize = config["signupview"]["info"]["fontsize"];

    // Offsets de renderizado

    // Username
    {
        username_box.x = config["signupview"]["username"]["offset"]["x"];
        username_box.y = config["signupview"]["username"]["offset"]["y"];
        username_box.w = config["signupview"]["username"]["w"];
        username_box.h = config["signupview"]["username"]["h"];
    }

    // Password
    {
        password_box.x = config["signupview"]["password"]["offset"]["x"];
        password_box.y = config["signupview"]["password"]["offset"]["y"];
        password_box.w = config["signupview"]["password"]["w"];
        password_box.h = config["signupview"]["password"]["h"];
    }

    // Race
    {
        race_box.prev_box.x =
            config["signupview"]["race"]["prev"]["offset"]["x"];
        race_box.prev_box.y =
            config["signupview"]["race"]["prev"]["offset"]["y"];
        race_box.prev_box.w = config["signupview"]["race"]["prev"]["w"];
        race_box.prev_box.h = config["signupview"]["race"]["prev"]["h"];

        race_box.bar_box.x = config["signupview"]["race"]["bar"]["offset"]["x"];
        race_box.bar_box.y = config["signupview"]["race"]["bar"]["offset"]["y"];
        race_box.bar_box.w = config["signupview"]["race"]["bar"]["w"];
        race_box.bar_box.h = config["signupview"]["race"]["bar"]["h"];

        race_box.next_box.x =
            config["signupview"]["race"]["next"]["offset"]["x"];
        race_box.next_box.y =
            config["signupview"]["race"]["next"]["offset"]["y"];
        race_box.next_box.w = config["signupview"]["race"]["next"]["w"];
        race_box.next_box.h = config["signupview"]["race"]["next"]["h"];
    }

    // Kind
    {
        kind_box.prev_box.x =
            config["signupview"]["kind"]["prev"]["offset"]["x"];
        kind_box.prev_box.y =
            config["signupview"]["kind"]["prev"]["offset"]["y"];
        kind_box.prev_box.w = config["signupview"]["kind"]["prev"]["w"];
        kind_box.prev_box.h = config["signupview"]["kind"]["prev"]["h"];

        kind_box.bar_box.x = config["signupview"]["kind"]["bar"]["offset"]["x"];
        kind_box.bar_box.y = config["signupview"]["kind"]["bar"]["offset"]["y"];
        kind_box.bar_box.w = config["signupview"]["kind"]["bar"]["w"];
        kind_box.bar_box.h = config["signupview"]["kind"]["bar"]["h"];

        kind_box.next_box.x =
            config["signupview"]["kind"]["next"]["offset"]["x"];
        kind_box.next_box.y =
            config["signupview"]["kind"]["next"]["offset"]["y"];
        kind_box.next_box.w = config["signupview"]["kind"]["next"]["w"];
        kind_box.next_box.h = config["signupview"]["kind"]["next"]["h"];
    }

    // Head
    {
        head_box.prev_box.x =
            config["signupview"]["head"]["prev"]["offset"]["x"];
        head_box.prev_box.y =
            config["signupview"]["head"]["prev"]["offset"]["y"];
        head_box.prev_box.w = config["signupview"]["head"]["prev"]["w"];
        head_box.prev_box.h = config["signupview"]["head"]["prev"]["h"];

        head_box.bar_box.x = config["signupview"]["head"]["bar"]["offset"]["x"];
        head_box.bar_box.y = config["signupview"]["head"]["bar"]["offset"]["y"];
        head_box.bar_box.w = config["signupview"]["head"]["bar"]["w"];
        head_box.bar_box.h = config["signupview"]["head"]["bar"]["h"];

        head_box.next_box.x =
            config["signupview"]["head"]["next"]["offset"]["x"];
        head_box.next_box.y =
            config["signupview"]["head"]["next"]["offset"]["y"];
        head_box.next_box.w = config["signupview"]["head"]["next"]["w"];
        head_box.next_box.h = config["signupview"]["head"]["next"]["h"];
    }

    // Body
    {
        body_box.prev_box.x =
            config["signupview"]["body"]["prev"]["offset"]["x"];
        body_box.prev_box.y =
            config["signupview"]["body"]["prev"]["offset"]["y"];
        body_box.prev_box.w = config["signupview"]["body"]["prev"]["w"];
        body_box.prev_box.h = config["signupview"]["body"]["prev"]["h"];

        body_box.bar_box.x = config["signupview"]["body"]["bar"]["offset"]["x"];
        body_box.bar_box.y = config["signupview"]["body"]["bar"]["offset"]["y"];
        body_box.bar_box.w = config["signupview"]["body"]["bar"]["w"];
        body_box.bar_box.h = config["signupview"]["body"]["bar"]["h"];

        body_box.next_box.x =
            config["signupview"]["body"]["next"]["offset"]["x"];
        body_box.next_box.y =
            config["signupview"]["body"]["next"]["offset"]["y"];
        body_box.next_box.w = config["signupview"]["body"]["next"]["w"];
        body_box.next_box.h = config["signupview"]["body"]["next"]["h"];
    }

    // Preview
    {
        preview_box.x = config["signupview"]["preview"]["offset"]["x"];
        preview_box.y = config["signupview"]["preview"]["offset"]["y"];
        preview_box.w = config["signupview"]["preview"]["w"];
        preview_box.h = config["signupview"]["preview"]["h"];
    }

    // Goback button
    {
        goback_box.x = config["signupview"]["goback"]["offset"]["x"];
        goback_box.y = config["signupview"]["goback"]["offset"]["y"];
        goback_box.w = config["signupview"]["goback"]["w"];
        goback_box.h = config["signupview"]["goback"]["h"];
    }

    // Create button
    {
        create_box.x = config["signupview"]["create"]["offset"]["x"];
        create_box.y = config["signupview"]["create"]["offset"]["y"];
        create_box.w = config["signupview"]["create"]["w"];
        create_box.h = config["signupview"]["create"]["h"];
    }

    // Info message
    {
        info_box.x = config["signupview"]["info"]["offset"]["x"];
        info_box.y = config["signupview"]["info"]["offset"]["y"];
        info_box.w = config["signupview"]["info"]["w"];
    }
}

void SignUpView::_loadPreviewData() {
    json signup_data = JSON::loadJsonFile(SIGNUP_DATA_FILEPATH);

    // Razas
    {
        json& races = signup_data["races"];
        size_t n_races = races.size();
        Id new_id;
        std::string name;
        for (size_t i = 0; i < n_races; i++) {
            new_id = (Id)races[i]["id"];
            name = (std::string)races[i]["name"];
            races_data.emplaceBack(new_id);
            SelectionBoxData& data = races_data.back();
            data.texture.loadFromRenderedText(&renderer, input_font, name,
                                              SDL_Color(VIEWS_FONT_COLOR));
            data.texture_pos.x =
                race_box.bar_box.x +
                (race_box.bar_box.w - data.texture.getWidth()) / 2;
            data.texture_pos.y =
                race_box.bar_box.y +
                (race_box.bar_box.h - data.texture.getHeight()) / 2;
        }
    }

    // Clases
    {
        json& kinds = signup_data["kinds"];
        size_t n_kinds = kinds.size();
        Id new_id;
        std::string name;
        for (size_t i = 0; i < n_kinds; i++) {
            new_id = (Id)kinds[i]["id"];
            name = (std::string)kinds[i]["name"];
            kinds_data.emplaceBack(new_id);
            SelectionBoxData& data = kinds_data.back();
            data.texture.loadFromRenderedText(&renderer, input_font, name,
                                              SDL_Color(VIEWS_FONT_COLOR));
            data.texture_pos.x =
                kind_box.bar_box.x +
                (kind_box.bar_box.w - data.texture.getWidth()) / 2;
            data.texture_pos.y =
                kind_box.bar_box.y +
                (kind_box.bar_box.h - data.texture.getHeight()) / 2;
        }
    }
}

void SignUpView::_loadMedia() {
    // Cargamos el background
    bg.loadFromFile(&renderer, SIGNUPVIEW_BG_FP);

    // Cargamos la/s fuente/s a usar
    input_font = TTF_OpenFont(VIEWS_INPUT_FONT, input_fontsize);
    cursor_font = TTF_OpenFont(VIEWS_CURSOR_FONT, input_fontsize);
    info_font = TTF_OpenFont(VIEWS_INFO_FONT, info_fontsize);

    if (!input_font || !cursor_font || !info_font) {
        throw Exception("SignUpView::loadMedia: Error opening TTF_Font/s.");
    }

    // Cargamos los sprites pertinentes
    sprites.loadHeadsMedia();
    sprites.loadBodiesMedia();

    // Cargamos la data para la preview
    _loadPreviewData();

    // Cargamos las imagenes estáticas

    // InputBox
    {
        input_inactive_box.loadFromFile(&renderer, SIGNUPVIEW_TEXTBOX_FP);
        input_active_box.loadFromFile(&renderer, SIGNUPVIEW_TEXTBOX_ACTIVE_FP);
    }

    // SelectionBox
    {
        prev.loadFromFile(&renderer, SIGNUPVIEW_SELECTIONBOX_PREV_FP);
        prev_pressed.loadFromFile(&renderer,
                                  SIGNUPVIEW_SELECTIONBOX_PREV_PRESSED_FP);

        next.loadFromFile(&renderer, SIGNUPVIEW_SELECTIONBOX_NEXT_FP);
        next_pressed.loadFromFile(&renderer,
                                  SIGNUPVIEW_SELECTIONBOX_NEXT_PRESSED_FP);

        bar.loadFromFile(&renderer, SIGNUPVIEW_SELECTIONBOX_BAR_FP);
        small_bar.loadFromFile(&renderer, SIGNUPVIEW_SELECTIONBOX_SMALL_BAR_FP);
    }

    // Botones
    {
        goback_button.loadFromFile(&renderer, SIGNUPVIEW_GOBACK_BUTTON_FP);
        goback_button_pressed.loadFromFile(&renderer,
                                           SIGNUPVIEW_GOBACK_BUTTON_PRESSED_FP);
        create_button.loadFromFile(&renderer, SIGNUPVIEW_CREATE_BUTTON_FP);
        create_button_pressed.loadFromFile(&renderer,
                                           SIGNUPVIEW_CREATE_BUTTON_PRESSED_FP);
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

void SignUpView::_processSDLEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        _handleEvent(e);
    }
}

void SignUpView::_render() const {
    SDL_Rect render_quad = {0};

    // Renderizamos el background
    {
        render_quad = {0, 0, bg.getWidth(), bg.getHeight()};
        renderer.render(bg.getTexture(), &render_quad);
    }

    // Renderizamos el username
    {
        render_quad = username_box;
        if (username_active) {
            renderer.render(input_active_box.getTexture(), &render_quad);
        } else {
            renderer.render(input_inactive_box.getTexture(), &render_quad);
        }

        render_quad = {username_pos.x, username_pos.y, username.getWidth(),
                       username.getHeight()};
        renderer.render(username.getTexture(), &render_quad);
    }

    // Renderizamos la contraseña
    {
        render_quad = password_box;
        if (password_active) {
            renderer.render(input_active_box.getTexture(), &render_quad);
        } else {
            renderer.render(input_inactive_box.getTexture(), &render_quad);
        }

        render_quad = {password_pos.x, password_pos.y, password.getWidth(),
                       password.getHeight()};
        renderer.render(password.getTexture(), &render_quad);
    }

    // Renderizamos race
    {
        render_quad = race_box.prev_box;
        if (race_box.prev_over) {
            renderer.render(prev_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(prev.getTexture(), &render_quad);
        }

        render_quad = race_box.next_box;
        if (race_box.next_over) {
            renderer.render(next_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(next.getTexture(), &render_quad);
        }

        render_quad = race_box.bar_box;
        renderer.render(bar.getTexture(), &render_quad);

        const SelectionBoxData& current_race = races_data.getCurrentValue();
        render_quad = {current_race.texture_pos.x, current_race.texture_pos.y,
                       current_race.texture.getWidth(),
                       current_race.texture.getHeight()};
        renderer.render(current_race.texture.getTexture(), &render_quad);
    }

    // Renderizamos kind
    {
        render_quad = kind_box.prev_box;
        if (kind_box.prev_over) {
            renderer.render(prev_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(prev.getTexture(), &render_quad);
        }

        render_quad = kind_box.next_box;
        if (kind_box.next_over) {
            renderer.render(next_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(next.getTexture(), &render_quad);
        }

        render_quad = kind_box.bar_box;
        renderer.render(bar.getTexture(), &render_quad);

        const SelectionBoxData& current_kind = kinds_data.getCurrentValue();
        render_quad = {current_kind.texture_pos.x, current_kind.texture_pos.y,
                       current_kind.texture.getWidth(),
                       current_kind.texture.getHeight()};
        renderer.render(current_kind.texture.getTexture(), &render_quad);
    }

    // Renderizamos head
    {
        render_quad = head_box.prev_box;
        if (head_box.prev_over) {
            renderer.render(prev_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(prev.getTexture(), &render_quad);
        }

        render_quad = head_box.next_box;
        if (head_box.next_over) {
            renderer.render(next_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(next.getTexture(), &render_quad);
        }

        render_quad = head_box.bar_box;
        renderer.render(small_bar.getTexture(), &render_quad);
    }

    // Renderizamos body
    {
        render_quad = body_box.prev_box;
        if (body_box.prev_over) {
            renderer.render(prev_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(prev.getTexture(), &render_quad);
        }

        render_quad = body_box.next_box;
        if (body_box.next_over) {
            renderer.render(next_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(next.getTexture(), &render_quad);
        }

        render_quad = body_box.bar_box;
        renderer.render(small_bar.getTexture(), &render_quad);
    }

    // Renderizamos botones
    {
        render_quad = goback_box;
        if (goback_button_over) {
            renderer.render(goback_button_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(goback_button.getTexture(), &render_quad);
        }

        render_quad = create_box;
        if (create_button_over) {
            renderer.render(create_button_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(create_button.getTexture(), &render_quad);
        }
    }

    // Renderizamos el mensaje de información
    {
        render_quad = {info_pos.x, info_pos.y, info_msg.getWidth(),
                       info_msg.getHeight()};
        renderer.render(info_msg.getTexture(), &render_quad);
    }

    // Renderizamos el cursor
    {
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
}

void SignUpView::_func(const int it) {
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

void SignUpView::_handleEvent(const SDL_Event& e) {
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
                _handleCreateButtonPressed();
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
                if (current_username.size() < MAX_USERNAME_SIZE) {
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
                        &renderer, info_font, SIGNUPVIEW_MAX_INPUT_MSG,
                        SDL_Color(VIEWS_WARNING_COLOR));
                    _setInfoPos();
                }

                break;
            }

            if (password_active) {
                if (current_password.size() < MAX_PASSWORD_SIZE) {
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
                        &renderer, info_font, SIGNUPVIEW_MAX_INPUT_MSG,
                        SDL_Color(VIEWS_WARNING_COLOR));
                    _setInfoPos();
                }

                break;
            }

            break;
        }

        case SDL_MOUSEMOTION: {
            SDL_Point mouse_pos = _getMousePos(e);

            // Boton de volver
            if (_inside(mouse_pos, goback_box)) {
                goback_button_over = true;
            } else {
                goback_button_over = false;
            }

            // Boton de crear
            if (_inside(mouse_pos, create_box)) {
                create_button_over = true;
            } else {
                create_button_over = false;
            }

            // Botones de selection-boxes

            if (_inside(mouse_pos, race_box.prev_box)) {
                race_box.prev_over = true;
            } else {
                race_box.prev_over = false;
            }

            if (_inside(mouse_pos, race_box.next_box)) {
                race_box.next_over = true;
            } else {
                race_box.next_over = false;
            }

            if (_inside(mouse_pos, kind_box.prev_box)) {
                kind_box.prev_over = true;
            } else {
                kind_box.prev_over = false;
            }

            if (_inside(mouse_pos, kind_box.next_box)) {
                kind_box.next_over = true;
            } else {
                kind_box.next_over = false;
            }

            if (_inside(mouse_pos, head_box.prev_box)) {
                head_box.prev_over = true;
            } else {
                head_box.prev_over = false;
            }

            if (_inside(mouse_pos, head_box.next_box)) {
                head_box.next_over = true;
            } else {
                head_box.next_over = false;
            }

            if (_inside(mouse_pos, body_box.prev_box)) {
                body_box.prev_over = true;
            } else {
                body_box.prev_over = false;
            }

            if (_inside(mouse_pos, body_box.next_box)) {
                body_box.next_over = true;
            } else {
                body_box.next_over = false;
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

            // Click en los botones de SelectionBox
            if (_inside(mouse_pos, race_box.next_box)) {
                races_data.next();
                break;
            }

            if (_inside(mouse_pos, race_box.prev_box)) {
                races_data.prev();
                break;
            }

            if (_inside(mouse_pos, kind_box.next_box)) {
                kinds_data.next();
                break;
            }

            if (_inside(mouse_pos, kind_box.prev_box)) {
                kinds_data.prev();
                break;
            }

            if (_inside(mouse_pos, head_box.next_box)) {
                fprintf(stderr, "Click en HEAD NEXT\n");
                break;
            }

            if (_inside(mouse_pos, head_box.prev_box)) {
                fprintf(stderr, "Click en HEAD PREV\n");
                break;
            }

            if (_inside(mouse_pos, body_box.next_box)) {
                fprintf(stderr, "Click en BODY NEXT\n");
                break;
            }

            if (_inside(mouse_pos, body_box.prev_box)) {
                fprintf(stderr, "Click en BODY PREV\n");
                break;
            }

            // Click en el botón de volver
            if (_inside(mouse_pos, goback_box)) {
                _handleGoBackButtonPressed();
                break;
            }

            // Click en el botón de crear cuenta
            if (_inside(mouse_pos, create_box)) {
                _handleCreateButtonPressed();
                break;
            }

            break;
        }

        default: {
            break;
        }
    }
}

void SignUpView::_handleCreateButtonPressed() {
    if (current_username.empty() || current_password.empty()) {
        info_msg.loadFromRenderedText(&renderer, info_font,
                                      SIGNUPVIEW_INVALID_INPUT_MSG,
                                      SDL_Color(VIEWS_ERROR_COLOR));
        _setInfoPos();
        return;
    }

    info_msg.loadFromRenderedText(&renderer, info_font, SIGNUPVIEW_CREATING_MSG,
                                  SDL_Color(VIEWS_FONT_COLOR));
    _setInfoPos();

    // INTENTAR CREAR EL PERSONAJE
    fprintf(stderr, "intentar crear personaje. aun no implementado\n");
}

void SignUpView::_handleGoBackButtonPressed() {
    current_context = CONNECTION_CTX;
    quit();
}

void SignUpView::_setInputPos() {
    // Centramos solo verticalmente
    username_pos.x = username_box.x + VIEWS_INPUT_TEXTBOX_X_OFFSET;
    username_pos.y =
        username_box.y + (username_box.h - username.getHeight()) / 2;

    password_pos.x = password_box.x + VIEWS_INPUT_TEXTBOX_X_OFFSET;
    password_pos.y =
        password_box.y + (password_box.h - password.getHeight()) / 2;
}

void SignUpView::_setInfoPos() {
    // Centramos horizontalmente
    info_pos.x = info_box.x + (info_box.w - info_msg.getWidth()) / 2;
    info_pos.y = info_box.y;
}

SDL_Point SignUpView::_getMousePos(const SDL_Event& e) const {
    return SDL_Point({((int)(e.button.x / renderer.getWidthScaleFactor())),
                      ((int)(e.button.y / renderer.getHeightScaleFactor()))});
}

bool SignUpView::_inside(const SDL_Point& pos, const SDL_Rect& box) const {
    if ((pos.x > (box.x + box.w)) || (pos.x < (box.x)) ||
        (pos.y > (box.y + box.h)) || (pos.y < (box.y))) {
        return false;
    }

    return true;
}

void SignUpView::_updateCursorAnimation(const int it) {
    if (username_active || password_active) {
        cursor_cooldown -= it;
        while (cursor_cooldown <= 0) {
            _switchCursorVisibility();
            cursor_cooldown += VIEWS_ITERATIONS_TO_SWITCH_CURSOR;
        }
    }
}

void SignUpView::_resetCursorCooldown() {
    show_cursor = true;
    cursor_cooldown = VIEWS_ITERATIONS_TO_SWITCH_CURSOR;
}

void SignUpView::_switchCursorVisibility() {
    if (show_cursor) {
        show_cursor = false;
    } else {
        show_cursor = true;
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

SignUpView::SignUpView(Context& current_context, Renderer& renderer,
                       const SocketWrapper& socket)
    : ConstantRateFunc(RATE),
      current_context(current_context),
      renderer(renderer),
      socket(socket),
      sprites(&renderer) {
    _init();
    _loadMedia();
    SDL_StartTextInput();
}

SignUpView::~SignUpView() {
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
