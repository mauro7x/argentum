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
    rate = 1000 / fps; /* ms por cada frame (floor) */

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

void GameView::_processSDLEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        event_handler.handleEvent(e);
    }
}

void GameView::_processServerUpdates() {
    /* Proxy */
    PlayerData* update = NULL;
    while ((update = broadcast.pop())) {
        fprintf(stderr, "Soy el cliente. Recibimos un update.\n");
        player.update(*update);
        delete update;
    }
}

void GameView::_loopIteration(const int it) {
    // auto t1 = std::chrono::steady_clock::now();
    /* Vaciamos las colas a procesar*/
    _processSDLEvents();
    _processServerUpdates();

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

    // auto t2 = std::chrono::steady_clock::now();
    // std::chrono::duration<float, std::micro> diff = t2 - t1;
    // fprintf(stderr, "Iteration time: %i us.\n", (int)diff.count());
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

GameView::GameView(BlockingQueue<Command*>& commands,
                   NonBlockingQueue<Update*>& updates, std::atomic_bool& exit)
    : commands(commands),
      updates(updates),
      exit(exit),
      renderer(window, camera),
      rate(0),
      hud(&renderer),
      console(&renderer),
      map(&renderer),
      unit_sprites(&renderer),
      player(&renderer, &unit_sprites),
      characters(&renderer, &unit_sprites),
      creatures(&renderer, &unit_sprites),

      server(requests, broadcast),

      stage(map, player, characters, creatures),
      event_handler(exit, requests) {}

void GameView::operator()() {
    // Iniciamos recursos necesarios
    _init();
    _loadMedia();

    server.start();  // proxy

    try {
        //-------------------------------------------------------------------------
        // PROXY PARA EL MANEJO DEL PRIMER PAQUETE DEL SERVER (hardcodeado).

        PlayerData init_data = {{1, 0, 0, DOWN_ORIENTATION},
                                100,
                                100,
                                100,
                                {InventorySlot({0, 0}), InventorySlot({0, 0}),
                                 InventorySlot({0, 0}), InventorySlot({0, 0}),
                                 InventorySlot({0, 0}), InventorySlot({0, 0}),
                                 InventorySlot({0, 0}), InventorySlot({0, 0}),
                                 InventorySlot({0, 0}), InventorySlot({0, 0})},
                                {0, 0, 0, 0},
                                2000,
                                2100,
                                1300,
                                1400,
                                1500,
                                1000};
        player.init(init_data);
        map.select(0); /* el id del mapa x ahora hardcodeado */
        //-------------------------------------------------------------------------

        // Variables para controlar el frame-rate
        auto t1 = std::chrono::steady_clock::now();
        auto t2 = t1;
        std::chrono::duration<float, std::milli> diff;
        int rest = 0, behind = 0, lost = 0;
        int it = 1;

        // Loop principal
        while (!exit) {
            _loopIteration(it);

            // Controlamos el rate y verificamos pérdida de frames.
            // Idea de implementación:
            // https://eldipa.github.io/book-of-gehn/articles/2019/10/23/Constant-Rate-Loop.html
            it = 0;
            t2 = std::chrono::steady_clock::now();
            diff = t2 - t1;
            rest = rate - std::ceil(diff.count());

            if (rest < 0) {
                fprintf(stderr, "\n\n=== PÉRDIDA DE FRAME/S ===\n\n\n");
                behind = -rest;
                lost = rate + (behind - behind % rate);
                rest = rate - behind % rate;
                t1 += std::chrono::milliseconds(lost);
                it += std::floor(lost / rate);
            }

            // fprintf(stderr, "MAIN-LOOP: Sleeping for %i ms.\n", rest);
            std::this_thread::sleep_for(std::chrono::milliseconds(rest));
            t1 += std::chrono::milliseconds(rate);
            it += 1;
        }
    } catch (const Exception& e) {
        server.kill();
        server.join();
        throw e;
    }

    server.kill();
    server.join();
}

GameView::~GameView() {
    IMG_Quit();
    SDL_Quit();
}

//-----------------------------------------------------------------------------
