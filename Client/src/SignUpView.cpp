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
        username_txtbx.render_box.x =
            config["signupview"]["username"]["offset"]["x"];
        username_txtbx.render_box.y =
            config["signupview"]["username"]["offset"]["y"];
        username_txtbx.render_box.w = config["signupview"]["username"]["w"];
        username_txtbx.render_box.h = config["signupview"]["username"]["h"];
    }

    // Password
    {
        password_txtbx.render_box.x =
            config["signupview"]["password"]["offset"]["x"];
        password_txtbx.render_box.y =
            config["signupview"]["password"]["offset"]["y"];
        password_txtbx.render_box.w = config["signupview"]["password"]["w"];
        password_txtbx.render_box.h = config["signupview"]["password"]["h"];
    }

    // Race
    {
        race_sibx.prev_box.x =
            config["signupview"]["race"]["prev"]["offset"]["x"];
        race_sibx.prev_box.y =
            config["signupview"]["race"]["prev"]["offset"]["y"];
        race_sibx.prev_box.w = config["signupview"]["race"]["prev"]["w"];
        race_sibx.prev_box.h = config["signupview"]["race"]["prev"]["h"];

        race_sibx.bar_box.x =
            config["signupview"]["race"]["bar"]["offset"]["x"];
        race_sibx.bar_box.y =
            config["signupview"]["race"]["bar"]["offset"]["y"];
        race_sibx.bar_box.w = config["signupview"]["race"]["bar"]["w"];
        race_sibx.bar_box.h = config["signupview"]["race"]["bar"]["h"];

        race_sibx.next_box.x =
            config["signupview"]["race"]["next"]["offset"]["x"];
        race_sibx.next_box.y =
            config["signupview"]["race"]["next"]["offset"]["y"];
        race_sibx.next_box.w = config["signupview"]["race"]["next"]["w"];
        race_sibx.next_box.h = config["signupview"]["race"]["next"]["h"];
    }

    // Kind
    {
        kind_sibx.prev_box.x =
            config["signupview"]["kind"]["prev"]["offset"]["x"];
        kind_sibx.prev_box.y =
            config["signupview"]["kind"]["prev"]["offset"]["y"];
        kind_sibx.prev_box.w = config["signupview"]["kind"]["prev"]["w"];
        kind_sibx.prev_box.h = config["signupview"]["kind"]["prev"]["h"];

        kind_sibx.bar_box.x =
            config["signupview"]["kind"]["bar"]["offset"]["x"];
        kind_sibx.bar_box.y =
            config["signupview"]["kind"]["bar"]["offset"]["y"];
        kind_sibx.bar_box.w = config["signupview"]["kind"]["bar"]["w"];
        kind_sibx.bar_box.h = config["signupview"]["kind"]["bar"]["h"];

        kind_sibx.next_box.x =
            config["signupview"]["kind"]["next"]["offset"]["x"];
        kind_sibx.next_box.y =
            config["signupview"]["kind"]["next"]["offset"]["y"];
        kind_sibx.next_box.w = config["signupview"]["kind"]["next"]["w"];
        kind_sibx.next_box.h = config["signupview"]["kind"]["next"]["h"];
    }

    // Head
    {
        head_sibx.prev_box.x =
            config["signupview"]["head"]["prev"]["offset"]["x"];
        head_sibx.prev_box.y =
            config["signupview"]["head"]["prev"]["offset"]["y"];
        head_sibx.prev_box.w = config["signupview"]["head"]["prev"]["w"];
        head_sibx.prev_box.h = config["signupview"]["head"]["prev"]["h"];

        head_sibx.bar_box.x =
            config["signupview"]["head"]["bar"]["offset"]["x"];
        head_sibx.bar_box.y =
            config["signupview"]["head"]["bar"]["offset"]["y"];
        head_sibx.bar_box.w = config["signupview"]["head"]["bar"]["w"];
        head_sibx.bar_box.h = config["signupview"]["head"]["bar"]["h"];

        head_sibx.next_box.x =
            config["signupview"]["head"]["next"]["offset"]["x"];
        head_sibx.next_box.y =
            config["signupview"]["head"]["next"]["offset"]["y"];
        head_sibx.next_box.w = config["signupview"]["head"]["next"]["w"];
        head_sibx.next_box.h = config["signupview"]["head"]["next"]["h"];
    }

    // Body
    {
        body_sibx.prev_box.x =
            config["signupview"]["body"]["prev"]["offset"]["x"];
        body_sibx.prev_box.y =
            config["signupview"]["body"]["prev"]["offset"]["y"];
        body_sibx.prev_box.w = config["signupview"]["body"]["prev"]["w"];
        body_sibx.prev_box.h = config["signupview"]["body"]["prev"]["h"];

        body_sibx.bar_box.x =
            config["signupview"]["body"]["bar"]["offset"]["x"];
        body_sibx.bar_box.y =
            config["signupview"]["body"]["bar"]["offset"]["y"];
        body_sibx.bar_box.w = config["signupview"]["body"]["bar"]["w"];
        body_sibx.bar_box.h = config["signupview"]["body"]["bar"]["h"];

        body_sibx.next_box.x =
            config["signupview"]["body"]["next"]["offset"]["x"];
        body_sibx.next_box.y =
            config["signupview"]["body"]["next"]["offset"]["y"];
        body_sibx.next_box.w = config["signupview"]["body"]["next"]["w"];
        body_sibx.next_box.h = config["signupview"]["body"]["next"]["h"];
    }

    // Preview
    {
        preview_tile.x = config["signupview"]["preview"]["offset"]["x"];
        preview_tile.y = config["signupview"]["preview"]["offset"]["y"];
        preview_tile.w = config["signupview"]["preview"]["w"];
        preview_tile.h = config["signupview"]["preview"]["h"];
    }

    // Goback button
    {
        goback_btn.render_box.x = config["signupview"]["goback"]["offset"]["x"];
        goback_btn.render_box.y = config["signupview"]["goback"]["offset"]["y"];
        goback_btn.render_box.w = config["signupview"]["goback"]["w"];
        goback_btn.render_box.h = config["signupview"]["goback"]["h"];
    }

    // Create button
    {
        create_btn.render_box.x = config["signupview"]["create"]["offset"]["x"];
        create_btn.render_box.y = config["signupview"]["create"]["offset"]["y"];
        create_btn.render_box.w = config["signupview"]["create"]["w"];
        create_btn.render_box.h = config["signupview"]["create"]["h"];
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
            RaceData& data = races_data.back();
            data.texture.loadFromRenderedText(&renderer, input_font, name,
                                              SDL_Color(VIEWS_FONT_COLOR));
            data.texture_pos.x =
                race_sibx.bar_box.x +
                (race_sibx.bar_box.w - data.texture.getWidth()) / 2;
            data.texture_pos.y =
                race_sibx.bar_box.y +
                (race_sibx.bar_box.h - data.texture.getHeight()) / 2;

            // Llenamos los sprites de head y body
            size_t n_heads = races[i]["head_ids"].size();
            for (size_t j = 0; j < n_heads; j++) {
                data.head_ids.emplaceBack((Id)races[i]["head_ids"][j]);
                SelectionBoxData& head_data = data.head_ids.back();
                head_data.texture.loadFromRenderedText(
                    &renderer, input_font, std::to_string(j + 1),
                    SDL_Color(VIEWS_FONT_COLOR));
                head_data.texture_pos.x =
                    head_sibx.bar_box.x +
                    (head_sibx.bar_box.w - head_data.texture.getWidth()) / 2;
                head_data.texture_pos.y =
                    head_sibx.bar_box.y +
                    (head_sibx.bar_box.h - head_data.texture.getHeight()) / 2;
            }

            size_t n_bodies = races[i]["body_ids"].size();
            for (size_t j = 0; j < n_bodies; j++) {
                data.body_ids.emplaceBack((Id)races[i]["body_ids"][j]);
                SelectionBoxData& body_data = data.body_ids.back();
                body_data.texture.loadFromRenderedText(
                    &renderer, input_font, std::to_string(j + 1),
                    SDL_Color(VIEWS_FONT_COLOR));
                body_data.texture_pos.x =
                    body_sibx.bar_box.x +
                    (body_sibx.bar_box.w - body_data.texture.getWidth()) / 2;
                body_data.texture_pos.y =
                    body_sibx.bar_box.y +
                    (body_sibx.bar_box.h - body_data.texture.getHeight()) / 2;
            }
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
                kind_sibx.bar_box.x +
                (kind_sibx.bar_box.w - data.texture.getWidth()) / 2;
            data.texture_pos.y =
                kind_sibx.bar_box.y +
                (kind_sibx.bar_box.h - data.texture.getHeight()) / 2;
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

    // TextBoxes
    {
        username_txtbx.base.loadFromFile(&renderer, SIGNUPVIEW_TEXTBOX_FP);
        password_txtbx.base.loadFromFile(&renderer, SIGNUPVIEW_TEXTBOX_FP);

        username_txtbx.active.loadFromFile(&renderer,
                                           SIGNUPVIEW_TEXTBOX_ACTIVE_FP);
        password_txtbx.active.loadFromFile(&renderer,
                                           SIGNUPVIEW_TEXTBOX_ACTIVE_FP);
    }

    // SelectionBoxes
    {
        race_sibx.prev.loadFromFile(&renderer, SIGNUPVIEW_SELECTIONBOX_PREV_FP);
        kind_sibx.prev.loadFromFile(&renderer, SIGNUPVIEW_SELECTIONBOX_PREV_FP);
        head_sibx.prev.loadFromFile(&renderer, SIGNUPVIEW_SELECTIONBOX_PREV_FP);
        body_sibx.prev.loadFromFile(&renderer, SIGNUPVIEW_SELECTIONBOX_PREV_FP);

        race_sibx.prev_pressed.loadFromFile(
            &renderer, SIGNUPVIEW_SELECTIONBOX_PREV_PRESSED_FP);
        kind_sibx.prev_pressed.loadFromFile(
            &renderer, SIGNUPVIEW_SELECTIONBOX_PREV_PRESSED_FP);
        head_sibx.prev_pressed.loadFromFile(
            &renderer, SIGNUPVIEW_SELECTIONBOX_PREV_PRESSED_FP);
        body_sibx.prev_pressed.loadFromFile(
            &renderer, SIGNUPVIEW_SELECTIONBOX_PREV_PRESSED_FP);

        race_sibx.next.loadFromFile(&renderer, SIGNUPVIEW_SELECTIONBOX_NEXT_FP);
        kind_sibx.next.loadFromFile(&renderer, SIGNUPVIEW_SELECTIONBOX_NEXT_FP);
        head_sibx.next.loadFromFile(&renderer, SIGNUPVIEW_SELECTIONBOX_NEXT_FP);
        body_sibx.next.loadFromFile(&renderer, SIGNUPVIEW_SELECTIONBOX_NEXT_FP);

        race_sibx.next_pressed.loadFromFile(
            &renderer, SIGNUPVIEW_SELECTIONBOX_NEXT_PRESSED_FP);
        kind_sibx.next_pressed.loadFromFile(
            &renderer, SIGNUPVIEW_SELECTIONBOX_NEXT_PRESSED_FP);
        head_sibx.next_pressed.loadFromFile(
            &renderer, SIGNUPVIEW_SELECTIONBOX_NEXT_PRESSED_FP);
        body_sibx.next_pressed.loadFromFile(
            &renderer, SIGNUPVIEW_SELECTIONBOX_NEXT_PRESSED_FP);

        race_sibx.bar.loadFromFile(&renderer, SIGNUPVIEW_SELECTIONBOX_BAR_FP);
        kind_sibx.bar.loadFromFile(&renderer, SIGNUPVIEW_SELECTIONBOX_BAR_FP);
        head_sibx.bar.loadFromFile(&renderer,
                                   SIGNUPVIEW_SELECTIONBOX_SMALL_BAR_FP);
        body_sibx.bar.loadFromFile(&renderer,
                                   SIGNUPVIEW_SELECTIONBOX_SMALL_BAR_FP);
    }

    // Botones
    {
        goback_btn.base.loadFromFile(&renderer, SIGNUPVIEW_GOBACK_BUTTON_FP);
        goback_btn.pressed.loadFromFile(&renderer,
                                        SIGNUPVIEW_GOBACK_BUTTON_PRESSED_FP);
        create_btn.base.loadFromFile(&renderer, SIGNUPVIEW_CREATE_BUTTON_FP);
        create_btn.pressed.loadFromFile(&renderer,
                                        SIGNUPVIEW_CREATE_BUTTON_PRESSED_FP);
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
    }

    // Renderizamos la contraseña
    {
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
    }

    // Renderizamos race
    {
        render_quad = race_sibx.prev_box;
        if (race_sibx.prev_over) {
            renderer.render(race_sibx.prev_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(race_sibx.prev.getTexture(), &render_quad);
        }

        render_quad = race_sibx.next_box;
        if (race_sibx.next_over) {
            renderer.render(race_sibx.next_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(race_sibx.next.getTexture(), &render_quad);
        }

        render_quad = race_sibx.bar_box;
        renderer.render(race_sibx.bar.getTexture(), &render_quad);

        const RaceData& current_race = races_data.getCurrentValue();
        render_quad = {current_race.texture_pos.x, current_race.texture_pos.y,
                       current_race.texture.getWidth(),
                       current_race.texture.getHeight()};
        renderer.render(current_race.texture.getTexture(), &render_quad);
    }

    // Renderizamos kind
    {
        render_quad = kind_sibx.prev_box;
        if (kind_sibx.prev_over) {
            renderer.render(kind_sibx.prev_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(kind_sibx.prev.getTexture(), &render_quad);
        }

        render_quad = kind_sibx.next_box;
        if (kind_sibx.next_over) {
            renderer.render(kind_sibx.next_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(kind_sibx.next.getTexture(), &render_quad);
        }

        render_quad = kind_sibx.bar_box;
        renderer.render(kind_sibx.bar.getTexture(), &render_quad);

        const SelectionBoxData& current_kind = kinds_data.getCurrentValue();
        render_quad = {current_kind.texture_pos.x, current_kind.texture_pos.y,
                       current_kind.texture.getWidth(),
                       current_kind.texture.getHeight()};
        renderer.render(current_kind.texture.getTexture(), &render_quad);
    }

    // Renderizamos head
    {
        render_quad = head_sibx.prev_box;
        if (head_sibx.prev_over) {
            renderer.render(head_sibx.prev_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(head_sibx.prev.getTexture(), &render_quad);
        }

        render_quad = head_sibx.next_box;
        if (head_sibx.next_over) {
            renderer.render(head_sibx.next_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(head_sibx.next.getTexture(), &render_quad);
        }

        render_quad = head_sibx.bar_box;
        renderer.render(head_sibx.bar.getTexture(), &render_quad);

        const SelectionBoxData& current_head =
            races_data.getCurrentValue().head_ids.getCurrentValue();
        render_quad = {current_head.texture_pos.x, current_head.texture_pos.y,
                       current_head.texture.getWidth(),
                       current_head.texture.getHeight()};
        renderer.render(current_head.texture.getTexture(), &render_quad);
    }

    // Renderizamos body
    {
        render_quad = body_sibx.prev_box;
        if (body_sibx.prev_over) {
            renderer.render(body_sibx.prev_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(body_sibx.prev.getTexture(), &render_quad);
        }

        render_quad = body_sibx.next_box;
        if (body_sibx.next_over) {
            renderer.render(body_sibx.next_pressed.getTexture(), &render_quad);
        } else {
            renderer.render(body_sibx.next.getTexture(), &render_quad);
        }

        render_quad = body_sibx.bar_box;
        renderer.render(body_sibx.bar.getTexture(), &render_quad);

        const SelectionBoxData& current_body =
            races_data.getCurrentValue().body_ids.getCurrentValue();
        render_quad = {current_body.texture_pos.x, current_body.texture_pos.y,
                       current_body.texture.getWidth(),
                       current_body.texture.getHeight()};
        renderer.render(current_body.texture.getTexture(), &render_quad);
    }

    // Renderizamos la preview
    {
        // Body
        Id body_id = races_data.getCurrentValue().body_ids.getCurrentValue().id;
        if (body_id) {
            const UnitSprite& sprite = sprites[body_id];
            _renderSprite(sprite);
        }

        // Head
        Id head_id = races_data.getCurrentValue().head_ids.getCurrentValue().id;
        if (head_id) {
            const UnitSprite& sprite = sprites[head_id];
            _renderSprite(sprite);
        }
    }

    // Renderizamos botones
    {
        render_quad = goback_btn.render_box;
        if (goback_btn.mouse_over) {
            renderer.render(goback_btn.pressed.getTexture(), &render_quad);
        } else {
            renderer.render(goback_btn.base.getTexture(), &render_quad);
        }

        render_quad = create_btn.render_box;
        if (create_btn.mouse_over) {
            renderer.render(create_btn.pressed.getTexture(), &render_quad);
        } else {
            renderer.render(create_btn.base.getTexture(), &render_quad);
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
            if (username_txtbx.is_active) {
                if (!username_txtbx.s_content.empty()) {
                    render_quad = {username_txtbx.render_pos.x +
                                       username_txtbx.content.getWidth(),
                                   username_txtbx.render_pos.y,
                                   cursor.getWidth(), cursor.getHeight()};
                } else {
                    render_quad = {username_txtbx.render_pos.x,
                                   username_txtbx.render_pos.y,
                                   cursor.getWidth(), cursor.getHeight()};
                }

                renderer.render(cursor.getTexture(), &render_quad);
            }

            if (password_txtbx.is_active) {
                if (!password_txtbx.s_content.empty()) {
                    render_quad = {password_txtbx.render_pos.x +
                                       password_txtbx.content.getWidth(),
                                   password_txtbx.render_pos.y,
                                   cursor.getWidth(), cursor.getHeight()};
                } else {
                    render_quad = {password_txtbx.render_pos.x,
                                   password_txtbx.render_pos.y,
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
                if (username_txtbx.s_content.size() < MAX_USERNAME_SIZE) {
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
                        &renderer, info_font, SIGNUPVIEW_MAX_INPUT_MSG,
                        SDL_Color(VIEWS_WARNING_COLOR));
                    _setInfoPos();
                }

                break;
            }

            if (password_txtbx.is_active) {
                if (password_txtbx.s_content.size() < MAX_PASSWORD_SIZE) {
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
            if (_inside(mouse_pos, goback_btn.render_box)) {
                goback_btn.mouse_over = true;
            } else {
                goback_btn.mouse_over = false;
            }

            // Boton de crear
            if (_inside(mouse_pos, create_btn.render_box)) {
                create_btn.mouse_over = true;
            } else {
                create_btn.mouse_over = false;
            }

            // Botones de selection-boxes

            if (_inside(mouse_pos, race_sibx.prev_box)) {
                race_sibx.prev_over = true;
            } else {
                race_sibx.prev_over = false;
            }

            if (_inside(mouse_pos, race_sibx.next_box)) {
                race_sibx.next_over = true;
            } else {
                race_sibx.next_over = false;
            }

            if (_inside(mouse_pos, kind_sibx.prev_box)) {
                kind_sibx.prev_over = true;
            } else {
                kind_sibx.prev_over = false;
            }

            if (_inside(mouse_pos, kind_sibx.next_box)) {
                kind_sibx.next_over = true;
            } else {
                kind_sibx.next_over = false;
            }

            if (_inside(mouse_pos, head_sibx.prev_box)) {
                head_sibx.prev_over = true;
            } else {
                head_sibx.prev_over = false;
            }

            if (_inside(mouse_pos, head_sibx.next_box)) {
                head_sibx.next_over = true;
            } else {
                head_sibx.next_over = false;
            }

            if (_inside(mouse_pos, body_sibx.prev_box)) {
                body_sibx.prev_over = true;
            } else {
                body_sibx.prev_over = false;
            }

            if (_inside(mouse_pos, body_sibx.next_box)) {
                body_sibx.next_over = true;
            } else {
                body_sibx.next_over = false;
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

            // Click en los botones de SelectionBox
            if (_inside(mouse_pos, race_sibx.next_box)) {
                races_data.next();
                break;
            }

            if (_inside(mouse_pos, race_sibx.prev_box)) {
                races_data.prev();
                break;
            }

            if (_inside(mouse_pos, kind_sibx.next_box)) {
                kinds_data.next();
                break;
            }

            if (_inside(mouse_pos, kind_sibx.prev_box)) {
                kinds_data.prev();
                break;
            }

            if (_inside(mouse_pos, head_sibx.next_box)) {
                races_data.getCurrentValue().head_ids.next();
                break;
            }

            if (_inside(mouse_pos, head_sibx.prev_box)) {
                races_data.getCurrentValue().head_ids.prev();
                break;
            }

            if (_inside(mouse_pos, body_sibx.next_box)) {
                races_data.getCurrentValue().body_ids.next();
                break;
            }

            if (_inside(mouse_pos, body_sibx.prev_box)) {
                races_data.getCurrentValue().body_ids.prev();
                break;
            }

            // Click en el botón de volver
            if (_inside(mouse_pos, goback_btn.render_box)) {
                _handleGoBackButtonPressed();
                break;
            }

            // Click en el botón de crear cuenta
            if (_inside(mouse_pos, create_btn.render_box)) {
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
    if (username_txtbx.s_content.empty() || password_txtbx.s_content.empty()) {
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

void SignUpView::_renderSprite(const UnitSprite& sprite) const {
    // Centramos el sprite en el tile
    int x = preview_tile.x + (preview_tile.w - (sprite.clip_w)) / 2;
    int y = preview_tile.y + (preview_tile.h * (0.8)) - (sprite.clip_h);

    // Armamos los SDL_Rect de renderizado
    SDL_Rect render_quad, render_clip;
    render_quad = {x, y, (int)(sprite.clip_w), (int)(sprite.clip_h)};
    render_clip = {0, 0, sprite.clip_w, sprite.clip_h};

    // Renderizamos
    renderer.render(sprite.texture.getTexture(), &render_quad, &render_clip);
}

void SignUpView::_setInputPos() {
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
    if (username_txtbx.is_active || password_txtbx.is_active) {
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
