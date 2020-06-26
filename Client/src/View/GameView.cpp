#include "../../includes/View/GameView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void GameView::_init() {
    /* Cargamos los archivos de configuración */
    json user_config = JSON::loadJsonFile(CONFIG_FILEPATH);
    json gui_config = JSON::loadJsonFile(GUI_CONFIG_FILEPATH);

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

    /* Iniciamos el sistema de TTF */
    if (TTF_Init() == -1) {
        throw SDLException("Error in function TTF_Init()\nSDL_Error: %s",
                           SDL_GetError());
    }

    /* Iniciamos el sistema de audio */
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        throw SDLException("Error in function Mix_OpenAudio()\nSDL_Error: %s",
                           SDL_GetError());
    }

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

    /* Iniciamos la ventana */
    window.init(gui_config["window"]);

    /* Iniciamos el renderer */
    renderer.init(gui_config["renderer"], scale_factor_w, scale_factor_h);

    /* Iniciamos la cámara */
    camera.init(gui_config["camera"]);

    /* Iniciamos la HUD */
    hud.init(gui_config["hud"]);

    /* Iniciamos el EventHandler */
    event_handler.init(gui_config, scale_factor_w, scale_factor_h);
}

void GameView::_loadMedia() {
    unit_sprites.loadMedia();
    item_sprites.loadMedia();
    player.loadMedia();
    characters.loadMedia();
    creatures.loadMedia();
    hud.loadMedia();
    map.loadMedia();
}

void GameView::_processSDLEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        event_handler.handleEvent(e);
    }
}

void GameView::_processMessages() {
    Message* message = NULL;
    while ((message = messages.pop())) {
        message->update(hud);
        delete message;
    }
}

void GameView::_processBroadcasts() {
    Broadcast* broadcast = NULL;
    while ((broadcast = broadcasts.pop())) {
        broadcast->exec(map, player, characters, creatures);
        delete broadcast;
    }
}

void GameView::_loopIteration(const int it) {
    /* Vaciamos las colas a procesar*/
    _processSDLEvents();

    // auto t1 = std::chrono::steady_clock::now();
    _processMessages();
    _processBroadcasts();

    /* Limpiamos la pantalla */
    renderer.clearScreen();

    /* Acciones previas al renderizado*/
    player.act(it);
    characters.act(it);
    creatures.act(it);
    camera.center(player.getBox(), map.widthInPx(), map.heightInPx());
    hud.update(it);

    /* Renderizamos y presentamos la pantalla */
    stage.render();
    hud.render();
    // auto t2 = std::chrono::steady_clock::now();

    renderer.presentScreen();

    // std::chrono::duration<float, std::milli> diff = t2 - t1;
    // fprintf(stderr, "Iteration time: %i ms.\n", (int)diff.count());
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

GameView::GameView(BlockingQueue<Command*>& commands,
                   NonBlockingQueue<Broadcast*>& broadcasts,
                   NonBlockingQueue<Message*>& messages,
                   std::atomic_bool& exit)
    :  // Comunicación entre hilos
      commands(commands),
      broadcasts(broadcasts),
      messages(messages),
      exit(exit),

      // Componentes SDL principales
      renderer(window, camera),

      // Contenedores de sprites
      unit_sprites(&renderer),
      item_sprites(&renderer),

      // Unidades
      player(&renderer, &unit_sprites),
      characters(&renderer, &unit_sprites),
      creatures(&renderer, &unit_sprites),

      // Componentes de la vista
      hud(&renderer, item_sprites, player),
      map(&renderer, item_sprites),

      // Otros
      stage(map, player, characters, creatures),
      event_handler(exit, commands, hud, map, camera) {}

void GameView::operator()() {
    // Iniciamos recursos necesarios
    _init();
    _loadMedia();

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
        rest = RATE - std::ceil(diff.count());

        if (rest < 0) {
            fprintf(stderr, "\n\n=== PÉRDIDA DE FRAME/S ===\n\n\n");
            behind = -rest;
            lost = RATE + (behind - behind % RATE);
            rest = RATE - behind % RATE;
            t1 += std::chrono::milliseconds(lost);
            it += std::floor(lost / RATE);
        }

        // fprintf(stderr, "MAIN-LOOP: Sleeping for %i ms.\n", rest);
        std::this_thread::sleep_for(std::chrono::milliseconds(rest));
        t1 += std::chrono::milliseconds(RATE);
        it += 1;
    }
}

GameView::~GameView() {
    // Es necesario liberar las fuentes utilizadas antes de llamar a TTF_Quit.
    player.free();
    characters.free();
    creatures.free();
    hud.free();

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

//-----------------------------------------------------------------------------
