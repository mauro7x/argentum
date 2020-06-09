#include "../../includes/View/GameView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void GameView::_init() {
    /* Cargamos el archivo de configuración */
    json config = _loadJsonFile(GUI_CONFIG_FILEPATH);

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

    json map_config = _loadJsonFile(MAPS_FILEPATH);

    /* Iniciamos el unit container */
    player.init(map_config["tilewidth"], map_config["tileheight"]);
    units.init(map_config["tilewidth"], map_config["tileheight"]);
}

json GameView::_loadJsonFile(std::string filepath) const {
    std::ifstream file(filepath);
    if (file.fail()) {
        throw Exception("Error opening file: %s", filepath);
    }

    json j;
    file >> j;
    if (file.fail()) {
        throw Exception("Error reading file: %s", filepath);
    }

    file.close();
    if (file.fail()) {
        throw Exception("Error closing file: %s", filepath);
    }

    return j;
}

void GameView::_loadMedia() {
    hud.loadMedia();
    map.loadMedia();
    unit_sprites.loadMedia();
}

void GameView::_handleEvent(const SDL_Event& e) {
    player.handleEvent(e);
    units.handleEvent(e);
}

void GameView::_free() {
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
      predictor(map),
      unit_sprites(&renderer),
      player(&renderer, unit_sprites, predictor),
      units(&renderer, unit_sprites),
      stage(hud, map, player, units) {}

void GameView::operator()() {
    _init();
    _loadMedia();

    //-------------------------------------------------------------------------
    // Manejar el primer paquete recibido, crear unidades dinamicas
    // necesarias

    player.update(10, 10);
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
        // ACCIONES

        map.select(0); /* el id del mapa x ahora hardcodeado */
        player.act();
        camera.center(player.getBox(), map.widthInPx(), map.heightInPx());
        //---------------------------------------------------------------------

        stage.render();
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
