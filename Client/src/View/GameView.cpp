#include "../../includes/View/GameView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void GameView::_init() {
    /* Cargamos el archivo de configuración */
    _loadConfig();

    /* Iniciamos el sistema de SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw SDLException("Error in function SDL_Init()\nSDL_Error: %s",
                           SDL_GetError());
    }

    /* Hint para el renderizado de texturas */
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        fprintf(stderr, "Warning: Linear texture filtering not enabled!\n");
    }

    /* Iniciamos el sistema de IMG */
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        throw SDLException("Error in function IMG_Init()\nSDL_Error: %s",
                           SDL_GetError());
    }

    /* Iniciamos la ventana */
    window.init(config["window"]);

    /* Iniciamos el renderer */
    renderer.init(config["renderer"]);

    /* Iniciamos la cámara */
    camera.init(config["camera"]);
}

void GameView::_loadConfig() {
    std::ifstream file(VIEW_CONFIG_FILEPATH);
    if (file.fail()) {
        throw Exception("Error opening file: %s", VIEW_CONFIG_FILEPATH);
    }

    file >> config;
    if (file.fail()) {
        throw Exception("Error reading configuration file.");
    }

    file.close();
}

void GameView::_loadMedia() {
    hud.loadMedia();
    map.loadMedia();
    player.loadMedia();
}

void GameView::_handleEvent(const SDL_Event& e) {
    player.handleEvent(e);
}

void GameView::_free() {
    renderer.free();
    window.free();

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
    : renderer(window, camera),
      sdl_running(false),
      img_running(false),
      hud(&renderer),
      map(&renderer),
      player(&renderer) {}

void GameView::operator()() {
    _init();
    _loadMedia();

    //-------------------------------------------------------------------------
    // Manejar el primer paquete recibido, crear unidades dinamicas
    // necesarias
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
        renderer.clearScreen();

        //---------------------------------------------------------------------
        // Acciones previas al renderizado

        player.move();
        camera.center(player.getBox(), MAP_WIDTH, MAP_HEIGHT);
        //---------------------------------------------------------------------

        //---------------------------------------------------------------------
        // Renderizamos

        map.render();
        player.render();
        hud.render();
        //---------------------------------------------------------------------

        renderer.presentScreen();

        // Delay para controlar el frame rate? por ahora usamos vsync
    }

    // Avisarle al server que nos desconectamos?

    _free();
}

GameView::~GameView() {
    _free();
}

//-----------------------------------------------------------------------------
