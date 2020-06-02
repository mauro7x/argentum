#include "View.h"

//-----------------------------------------------------------------------------
// Métodos privados

void View::_init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw SDLException("Error in init: SDL_Init()", SDL_GetError());
    }

    // Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        fprintf(stderr, "Warning: Linear texture filtering not enabled!\n");
    }

    // Window

    mWindow = SDL_CreateWindow(WINDOW_TITLE, WINDOW_POS_X, WINDOW_POS_Y,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (mWindow == NULL) {
        throw SDLException("Error in window init: SDL_CreateWindow()",
                           SDL_GetError());
    }

    // Renderer

    Uint32 renderer_flags =
        VSYNC_ENABLED ? (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
                      : (SDL_RENDERER_ACCELERATED);

    mRenderer = SDL_CreateRenderer(mWindow, -1, renderer_flags);
    if (mRenderer == NULL) {
        throw SDLException("Error in window init: SDL_CreateRenderer()",
                           SDL_GetError());
    }

    if (SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF)) {
        throw SDLException("Error in window init: SDL_SetRenderDrawColor()",
                           SDL_GetError());
    }

    // PNG Loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        throw SDLException("Error in window init: IMG_Init()", SDL_GetError());
    }
}

void View::_createEntities() {
    // Crear entidades
    mEntities.add(new Foo(mRenderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
}

void View::_loadMedia() const {
    mEntities.loadMedia();
}

void View::_handleEvent(const SDL_Event& e) {
    mEntities.handleEvent(e);
}

void View::_clear() const {
    if (SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF)) {
        throw SDLException("Error in window clear: SDL_SetRenderDrawColor()",
                           SDL_GetError());
    }

    if (SDL_RenderClear(mRenderer)) {
        throw SDLException("Error in window clear: SDL_RenderClear()",
                           SDL_GetError());
    }
}

void View::_act() const {
    mEntities.act();
}

void View::_render() const {
    mEntities.render();
}

void View::_present() const {
    SDL_RenderPresent(mRenderer);
}

void View::_free() {
    mEntities.free();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

View::View() : mWindow(NULL), mRenderer(NULL) {}

void View::operator()() {
    _init();
    _createEntities();
    _loadMedia();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            _handleEvent(e);
        }

        // Game loop
        _clear();
        _act();
        _render();
        _present();
    }
}

View::~View() {
    _free();

    IMG_Quit();
    SDL_Quit();
}

//-----------------------------------------------------------------------------
