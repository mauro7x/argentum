#include "../../includes/View/GameView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void GameView::_init() {
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

    /* Iniciamos la ventana y su renderer */
    window.init();
    renderer.init();
}

void GameView::_handleEvent(const SDL_Event& e) {}

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
    : renderer(window), sdl_running(false), img_running(false) {}

void GameView::operator()() {
    /* Iniciamos subsistemas necesarios para SDL */
    _init();

    //-------------------------------------------------------------------------
    // Instancio objetos estáticos

    /* Iniciamos la interfaz HUD (versión Proxy) */
    HUDProxy hud(&renderer);
    hud.loadMedia();

    /* Iniciamos el mapa (versión Proxy) */
    MapProxy map(&renderer);
    map.loadMedia();

    /* Iniciamos al jugador principal */
    Player player(&renderer, 5, 5);
    player.loadMedia();
    //-------------------------------------------------------------------------

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

            // Pasamos el evento a nuestras entidades
            player.handleEvent(e);

            // Camara move
            /*
            if (e.type == SDL_KEYDOWN) {
                // Adjust the velocity
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        camera.y -= 5;
                        break;

                    case SDLK_DOWN:
                        camera.y += 5;
                        break;

                    case SDLK_LEFT:
                        camera.x -= 5;
                        break;

                    case SDLK_RIGHT:
                        camera.x += 5;
                        break;
                }

                // Arreglamos la camara por si la rompimo
                if (camera.x < 0) {
                    camera.x = 0;
                }
                if (camera.y < 0) {
                    camera.y = 0;
                }
                if (camera.x > MAP_WIDTH - camera.w) {
                    camera.x = MAP_WIDTH - camera.w;
                }
                if (camera.y > MAP_HEIGHT - camera.h) {
                    camera.y = MAP_HEIGHT - camera.h;
                }

                fprintf(stderr, "CAMERA STATUS: x = %i, y = %i\n", camera.x,
                        camera.y);
            }
            */
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
