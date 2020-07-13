#include "../includes/TutorialView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void TutorialView::_init() {
    json config = JSON::loadJsonFile(
        paths::config(VIEWS_CONFIG_FILEPATH))["tutorialview"];

    // Offsets de renderizado
    img_box.x = config["img_box"]["offset"]["x"];
    img_box.y = config["img_box"]["offset"]["y"];
    img_box.w = config["img_box"]["w"];
    img_box.h = config["img_box"]["h"];

    goback_btn.render_box.x = config["goback"]["offset"]["x"];
    goback_btn.render_box.y = config["goback"]["offset"]["y"];
    goback_btn.render_box.w = config["goback"]["w"];
    goback_btn.render_box.h = config["goback"]["h"];

    prev_btn.render_box.x = config["prev"]["offset"]["x"];
    prev_btn.render_box.y = config["prev"]["offset"]["y"];
    prev_btn.render_box.w = config["prev"]["w"];
    prev_btn.render_box.h = config["prev"]["h"];

    next_btn.render_box.x = config["next"]["offset"]["x"];
    next_btn.render_box.y = config["next"]["offset"]["y"];
    next_btn.render_box.w = config["next"]["w"];
    next_btn.render_box.h = config["next"]["h"];
}

void TutorialView::_loadMedia() {
    // Cargamos el background
    bg.loadFromFile(&renderer, paths::asset(TUTORIALVIEW_BG_FP));

    // Cargamos las imagenes estáticas
    goback_btn.base.loadFromFile(&renderer,
                                 paths::asset(TUTORIALVIEW_GOBACK_BUTTON_FP));
    goback_btn.pressed.loadFromFile(
        &renderer, paths::asset(TUTORIALVIEW_GOBACK_BUTTON_PRESSED_FP));

    prev_btn.base.loadFromFile(&renderer,
                               paths::asset(TUTORIALVIEW_PREV_BUTTON_FP));
    prev_btn.pressed.loadFromFile(
        &renderer, paths::asset(TUTORIALVIEW_PREV_BUTTON_PRESSED_FP));

    next_btn.base.loadFromFile(&renderer,
                               paths::asset(TUTORIALVIEW_NEXT_BUTTON_FP));
    next_btn.pressed.loadFromFile(
        &renderer, paths::asset(TUTORIALVIEW_NEXT_BUTTON_PRESSED_FP));

    // Cargamos las distintas imagenes del tutorial
    {
        json imgs_config = JSON::loadJsonFile(
            paths::config(VIEWS_CONFIG_FILEPATH))["tutorialview"]["imgs"];

        std::string dirpath, filepath;
        dirpath = imgs_config["dirpath"];
        dirpath = paths::asset(dirpath.c_str());
        size_t size = imgs_config["size"];

        for (size_t i = 0; i < size; i++) {
            filepath = dirpath + std::to_string(i) + ".png";
            imgs.emplace_back();
            imgs.back().loadFromFile(&renderer, filepath);
        }

        it = imgs.begin();
    }
}

void TutorialView::_processSDLEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        _handleEvent(e);

        // Propagamos el evento al mixer
        Mixer::handleEvent(e);
    }
}

void TutorialView::_render() const {
    // Renderizamos el background
    SDL_Rect render_quad = {0, 0, bg.getWidth(), bg.getHeight()};
    renderer.render(bg.getTexture(), &render_quad);

    // Renderizamos el boton de volver
    render_quad = goback_btn.render_box;
    if (goback_btn.mouse_over) {
        renderer.render(goback_btn.pressed.getTexture(), &render_quad);
    } else {
        renderer.render(goback_btn.base.getTexture(), &render_quad);
    }

    // Renderizamos el boton de prev
    render_quad = prev_btn.render_box;
    if (prev_btn.mouse_over) {
        renderer.render(prev_btn.pressed.getTexture(), &render_quad);
    } else {
        renderer.render(prev_btn.base.getTexture(), &render_quad);
    }

    // Renderizamos el boton de next
    render_quad = next_btn.render_box;
    if (next_btn.mouse_over) {
        renderer.render(next_btn.pressed.getTexture(), &render_quad);
    } else {
        renderer.render(next_btn.base.getTexture(), &render_quad);
    }

    // Renderizamos la imagen actual
    render_quad = img_box;
    renderer.render(it->getTexture(), &render_quad);
}

void TutorialView::_func(const int it) {
    /* Vaciamos las colas a procesar*/
    _processSDLEvents();

    /* Limpiamos la pantalla */
    renderer.clearScreen();

    /* Renderizamos y presentamos la pantalla */
    _render();

    /* Presentamos la pantalla */
    renderer.presentScreen();
}

//-----------------------------------------------------------------------------
// Auxiliares

void TutorialView::_handleEvent(const SDL_Event& e) {
    switch (e.type) {
        case SDL_QUIT: {
            current_context = EXIT_CTX;
            quit();
            break;
        }

        case SDL_KEYDOWN: {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                current_context = EXIT_CTX;
                quit();
                break;
            }

            break;
        }

        case SDL_MOUSEMOTION: {
            SDL_Point mouse_pos = _getMousePos(e);
            if (_inside(mouse_pos, goback_btn.render_box)) {
                goback_btn.mouse_over = true;
            } else {
                goback_btn.mouse_over = false;
            }

            if (_inside(mouse_pos, prev_btn.render_box)) {
                prev_btn.mouse_over = true;
            } else {
                prev_btn.mouse_over = false;
            }

            if (_inside(mouse_pos, next_btn.render_box)) {
                next_btn.mouse_over = true;
            } else {
                next_btn.mouse_over = false;
            }

            break;
        }

        case SDL_MOUSEBUTTONDOWN: {
            if (e.button.button != SDL_BUTTON_LEFT) {
                break;
            }

            SDL_Point mouse_pos = _getMousePos(e);
            if (_inside(mouse_pos, goback_btn.render_box)) {
                _handleGoBackButtonPressed();
                Mixer::playLocalSound(CLICK_SOUND);
            }

            if (_inside(mouse_pos, prev_btn.render_box)) {
                if (it != imgs.begin()) {
                    it--;
                }
                Mixer::playLocalSound(CLICK_SOUND);
            }

            if (_inside(mouse_pos, next_btn.render_box)) {
                it++;
                if (it == imgs.end()) {
                    it--;
                }
                Mixer::playLocalSound(CLICK_SOUND);
            }

            break;
        }

        default: {
            break;
        }
    }
}

void TutorialView::_handleGoBackButtonPressed() {
    current_context = HOME_CTX;
    quit();
}

SDL_Point TutorialView::_getMousePos(const SDL_Event& e) const {
    return SDL_Point({((int)(e.button.x / renderer.getWidthScaleFactor())),
                      ((int)(e.button.y / renderer.getHeightScaleFactor()))});
}

bool TutorialView::_inside(const SDL_Point& pos, const SDL_Rect& box) const {
    if ((pos.x > (box.x + box.w)) || (pos.x < (box.x)) ||
        (pos.y > (box.y + box.h)) || (pos.y < (box.y))) {
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

TutorialView::TutorialView(Context& current_context, const Renderer& renderer)
    : current_context(current_context), renderer(renderer) {
    _init();
    _loadMedia();
}

TutorialView::~TutorialView() {}

//-----------------------------------------------------------------------------
