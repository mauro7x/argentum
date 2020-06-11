#include "../../includes/View/GameView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void GameView::_init() {
    /* Cargamos el archivo de configuración */
    json config = JSON::loadJsonFile(GUI_CONFIG_FILEPATH);

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
    int fps = config["fps"];
    rate = 1000 / fps;

    /* Iniciamos la ventana */
    window.init(config["window"]);

    /* Iniciamos el renderer */
    renderer.init(config["renderer"]);

    /* Iniciamos la cámara */
    camera.init(config["camera"]);
}

void GameView::_loadMedia() {
    hud.loadMedia();
    map.loadMedia();
    unit_sprites.loadMedia();
}

void GameView::_handleEvent(const SDL_Event& e) {
    /* Movimiento */
    int* cmd = NULL;

    // If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        // Adjust the velocity
        switch (e.key.keysym.sym) {
            case SDLK_w:
                cmd = new int(0);
                fprintf(stderr, "Soy el cliente. Enviamos un %i.\n", *cmd);
                requests.push(cmd);
                break;

            case SDLK_s:
                cmd = new int(1);
                fprintf(stderr, "Soy el cliente. Enviamos un %i.\n", *cmd);
                requests.push(cmd);
                break;

            case SDLK_a:
                cmd = new int(2);
                fprintf(stderr, "Soy el cliente. Enviamos un %i.\n", *cmd);
                requests.push(cmd);
                break;

            case SDLK_d:
                cmd = new int(3);
                fprintf(stderr, "Soy el cliente. Enviamos un %i.\n", *cmd);
                requests.push(cmd);
                break;
        }
    }
    // If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        // Adjust the velocity
        switch (e.key.keysym.sym) {
            case SDLK_w:
            case SDLK_s:
            case SDLK_a:
            case SDLK_d:
                cmd = new int(4);
                fprintf(stderr, "Soy el cliente. Enviamos un %i.\n", *cmd);
                requests.push(cmd);
                break;
        }
    }
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
      rate(0),
      sdl_running(false),
      img_running(false),
      fullscreen(false),
      hud(&renderer),
      map(&renderer),
      unit_sprites(&renderer),
      player(&renderer, unit_sprites),

      server(requests, broadcast),

      stage(hud, map, player) {}

void GameView::operator()() {
    _init();
    _loadMedia();
    server.start();

    //-------------------------------------------------------------------------
    // Manejar el primer paquete recibido, crear unidades
    // necesarias

    // Hardcodeamos el primer paquete
    PlayerData init_data = {0,    0,    DOWN, 100,  100, 2000,
                            2100, 1300, 1400, 1500, 1000};
    player.init(init_data);
    map.select(1); /* el id del mapa x ahora hardcodeado */
    //-------------------------------------------------------------------------

    bool quit = false;
    SDL_Event e;
    PlayerData* update = NULL;

    /* Variables para el control del frame-rate */
    uint32_t t1 = SDL_GetTicks(), t2 = 0, behind = 0, lost = 0;
    int rest = 0;
    uint32_t it = 0;

    while (!quit) {
        /* Manejamos eventos del usuario */
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            _handleEvent(e);
        }

        /* Manejamos updates del servidor */
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
        renderer.presentScreen();

        /* Controlamos el frame-rate */
        t2 = SDL_GetTicks();
        rest = rate - (t2 - t1);

        if (rest < 0) {
            behind = -rest;
            lost = (behind - behind % rate);
            rest = rate - behind % rate;
            t1 += lost;
            it += (lost / rate);
        }

        // fprintf(stderr, "Me duermo por %i ms. Iteracion: %i\n", rest, it +
        // 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(rest));
        t1 += rate;
        it++;
    }

    // Avisarle al server que nos desconectamos?

    server.kill();
    server.join();
    _free();
}

GameView::~GameView() {
    _free();
}

//-----------------------------------------------------------------------------
