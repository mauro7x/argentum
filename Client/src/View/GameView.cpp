#include "../../includes/View/GameView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void GameView::_init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw SDLException("Error in function SDL_Init()\nSDL_Error: %s",
                           SDL_GetError());
    }

    // Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        fprintf(stderr, "Warning: Linear texture filtering not enabled!\n");
    }

    // Window

    mWindow = SDL_CreateWindow(WINDOW_TITLE, WINDOW_POS_X, WINDOW_POS_Y,
                               WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (mWindow == NULL) {
        throw SDLException(
            "Error in function SDL_CreateWindow()\nSDL_Error: %s",
            SDL_GetError());
    }

    // Renderer

    Uint32 renderer_flags =
        VSYNC_ENABLED ? (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
                      : (SDL_RENDERER_ACCELERATED);

    mRenderer = SDL_CreateRenderer(mWindow, -1, renderer_flags);
    if (mRenderer == NULL) {
        throw SDLException(
            "Error in function SDL_CreateRenderer()\nSDL_Error: %s",
            SDL_GetError());
    }

    if (SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF)) {
        throw SDLException(
            "Error in function SDL_SetRenderDrawColor()\nSDL_Error: %s",
            SDL_GetError());
    }

    // PNG Loading
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        throw SDLException("Error in function IMG_Init()\nSDL_Error: %s",
                           SDL_GetError());
    }
}

void GameView::_loadMedia() {}

void GameView::_handleEvent(const SDL_Event& e) {}

void GameView::_clear() const {
    if (SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF)) {
        throw SDLException(
            "Error in function SDL_SetRenderDrawColor()\nSDL_Error: %s",
            SDL_GetError());
    }

    if (SDL_RenderClear(mRenderer)) {
        throw SDLException("Error in function SDL_RenderClear()\nSDL_Error: %s",
                           SDL_GetError());
    }
}

void GameView::_act() const {}

void GameView::_present() const {
    SDL_RenderPresent(mRenderer);
}

void GameView::_free() {
    if (mRenderer) {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = NULL;
    }

    if (mWindow) {
        SDL_DestroyWindow(mWindow);
        mWindow = NULL;
    }

    if (img_running) {
        IMG_Quit();
        img_running = false;
    }

    if (sdl_running) {
        SDL_Quit();
        sdl_running = false;
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

GameView::GameView()
    : mWindow(NULL), mRenderer(NULL), sdl_running(false), img_running(false) {}

void GameView::operator()() {
    _init();
    // Manejar el primer paquete recibido, crear unidades dinamicas necesarias
    _loadMedia();

    //-------------------------------------------------------------------------
    // Instancio objetos estáticos

    SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    Texture bg(mRenderer);
    bg.loadFromFile("../Assets/game-view.png");

    MapProxy map(mRenderer);

    //-------------------------------------------------------------------------

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Handle user-events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            _handleEvent(e);
        }

        /*
        // Handle server-updates on queue
        while (hayan updates que realizar) {
            realizar update en mis entidades
        }
        */

        // Game loop
        _clear();

        //---------------------------------------------------------------------
        // Acciones previas al renderizado

        _act();
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        // Renderizamos

        bg.render(0, 0);
        map.render(camera);
        //---------------------------------------------------------------------

        _present();

        // Delay para controlar el frame rate?
    }

    // Avisarle al server que nos desconectamos?

    _free();
}

GameView::~GameView() {
    _free();
}

//-----------------------------------------------------------------------------
