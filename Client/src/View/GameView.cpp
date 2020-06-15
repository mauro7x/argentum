#include "../../includes/View/GameView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void GameView::_init() {
    /* Cargamos los archivos de configuración */
    json user_config = JSON::loadJsonFile(CONFIG_FILEPATH);
    json gui_config = JSON::loadJsonFile(GUI_CONFIG_FILEPATH);
    json map_config = JSON::loadJsonFile(MAPS_FILEPATH);
    json common_config = JSON::loadJsonFile(COMMON_CONFIG_FILEPATH);

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

    /* Setteamos el frame-rate */
    int fps = gui_config["fps"];
    rate = 1000 / fps;

    /* Cargamos las dimensiones de los tiles */
    int tile_w = map_config["tilewidth"];
    int tile_h = map_config["tileheight"];

    /* Calculamos los factores de escala */
    float scale_factor_w, scale_factor_h;
    int original_w, original_h, new_w, new_h;
    original_w = gui_config["window"]["w"];
    original_h = gui_config["window"]["h"];
    new_w = user_config["size"]["w"];
    new_h = user_config["size"]["h"];

    bool fullscreen = user_config["size"]["fullscreen"];
    if (fullscreen) {
        SDL_DisplayMode dm;
        if (SDL_GetCurrentDisplayMode(0, &dm)) {
            throw SDLException(
                "Error in function SDL_GetCurrentDisplayMode()\nSDL_Error: %s",
                SDL_GetError());
        }
        new_w = dm.w;
        new_h = dm.h;
    }

    scale_factor_w = (float)new_w / original_w;
    scale_factor_h = (float)new_h / original_h;

    /* Ajustamos el ancho y alto de la ventana */
    gui_config["window"]["fullscreen"] = fullscreen;
    gui_config["window"]["w"] = (int)(original_w * scale_factor_w);
    gui_config["window"]["h"] = (int)(original_h * scale_factor_h);

    /* Cargamos la velocidad de movimiento de las unidades */
    int speed = common_config["tiles_per_sec"]["character_speed"];
    float tile_movement_time = 1000 / speed;

    /* Iniciamos la ventana */
    window.init(gui_config["window"]);

    /* Iniciamos el renderer */
    renderer.init(gui_config["renderer"], scale_factor_w, scale_factor_h);

    /* Iniciamos la cámara */
    camera.init(gui_config["camera"], tile_w, tile_h);

    /* Iniciamos la HUD */
    hud.init(gui_config["hud"]);

    /* Iniciamos la consola */
    console.init(gui_config["console"]);

    /* Iniciamos los contenedores */
    characters.init(tile_w, tile_h, tile_movement_time);
    creatures.init(tile_w, tile_h, tile_movement_time);
}

void GameView::_loadMedia() {
    hud.loadMedia();
    console.loadMedia();
    map.loadMedia();
    unit_sprites.loadMedia();
}

void GameView::_gameIteration(uint32_t it) {
    /* Manejamos updates del servidor */
    PlayerData* update = NULL;
    while ((update = broadcast.pop())) {
        fprintf(stderr, "Soy el cliente. Recibimos un update.\n");
        player.update(*update);
        delete update;
    }

    /* Limpiamos la pantalla */
    renderer.clearScreen();

    /* Acciones previas al renderizado*/
    player.act(it);
    camera.center(player.getBox(), map.widthInPx(), map.heightInPx());

    /* Renderizamos y presentamos la pantalla */
    stage.render();
    hud.render();
    console.render();
    renderer.presentScreen();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

GameView::GameView()
    : renderer(window, camera),
      rate(0),
      view_running(true),
      hud(&renderer),
      console(&renderer),
      map(&renderer),
      unit_sprites(&renderer),
      player(&renderer, &unit_sprites),
      characters(&renderer, &unit_sprites),
      creatures(&renderer, &unit_sprites),

      server(requests, broadcast),

      stage(map, player, characters, creatures),
      event_handler(view_running, requests) {}

void GameView::operator()() {
    /* Inicializamos variables internas, hilos de ejecución y cargamos media */
    _init();
    _loadMedia();
    server.start();  // proxy
    event_handler.start();

    try {
        //-------------------------------------------------------------------------
        // PROXY PARA EL MANEJO DEL PRIMER PAQUETE DEL SERVER (hardcodeado).

        PlayerData init_data = {
            {1, 0, 0, DOWN}, 100, 100, 100, 2000, 2100, 1300, 1400, 1500, 0};
        player.init(init_data);
        map.select(0); /* el id del mapa x ahora hardcodeado */
        //-------------------------------------------------------------------------

        /* Variables para el control del frame-rate */
        uint32_t t1 = SDL_GetTicks(), t2 = 0, behind = 0, lost = 0, it = 0;
        int rest = 0;

        /* Loop principal del juego (acciones y renderizado) */
        while (view_running) {
            /* Ejecución de la iteración del juego */
            _gameIteration(it);

            /* Control del frame-rate */
            t2 = SDL_GetTicks();
            rest = rate - (t2 - t1);

            if (rest < 0) {
                fprintf(stderr, "\n\n== PERDIDA DE FRAME/S ==\n\n\n");
                behind = -rest;
                lost = (behind - behind % rate);
                rest = rate - behind % rate;
                t1 += lost;
                it += (lost / rate);
            }

            // fprintf(stderr, "MAIN-LOOP | It: %i | Sleep: %i ms\n", it + 1,
            //        rest);
            std::this_thread::sleep_for(std::chrono::milliseconds(rest));
            t1 += rate;
            it++;
        }
    } catch (const Exception& e) {
        view_running = false;
        event_handler.join();
        server.kill();
        server.join();
        throw e;
    }

    // Avisarle al server que nos desconectamos?

    event_handler.join();
    server.kill();
    server.join();
}

GameView::~GameView() {
    IMG_Quit();
    SDL_Quit();
}

//-----------------------------------------------------------------------------
