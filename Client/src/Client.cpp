#include "../includes/Client.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Client::_initSDL() {
    /* Iniciamos el sistema de SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw Exception("Error in function SDL_Init()\nSDL_Error: %s",
                        SDL_GetError());
    }

    /* Hint para el renderizado de texturas */
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        fprintf(stderr, "Warning: Linear texture filtering not enabled!\n");
    }

    /* Iniciamos el sistema de IMG */
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        throw Exception("Error in function IMG_Init()\nSDL_Error: %s",
                        SDL_GetError());
    }

    /* Iniciamos el sistema de TTF */
    if (TTF_Init() == -1) {
        throw Exception("Error in function TTF_Init()\nSDL_Error: %s",
                        SDL_GetError());
    }

    /* Iniciamos el sistema de audio */
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        throw Exception("Error in function Mix_OpenAudio()\nSDL_Error: %s",
                        SDL_GetError());
    }
}

void Client::_initComponents() {
    /* Cargamos los archivos de configuración */
    json config = JSON::loadJsonFile(CONFIG_FILEPATH);
    json user_config = JSON::loadJsonFile(USER_CONFIG_FILEPATH);

    /* Calculamos los factores de escala */
    float scale_factor_w, scale_factor_h;
    int original_w, original_h, new_w, new_h;
    original_w = config["window"]["w"];
    original_h = config["window"]["h"];
    new_w = user_config["window"]["w"];
    new_h = user_config["window"]["h"];

    bool fullscreen = user_config["window"]["fullscreen"];
    if (fullscreen) {
        SDL_DisplayMode dm;
        if (SDL_GetCurrentDisplayMode(0, &dm)) {
            throw Exception(
                "Error in function SDL_GetCurrentDisplayMode()\nSDL_Error: %s",
                SDL_GetError());
        }
        new_w = dm.w;
        new_h = dm.h;
    }

    scale_factor_w = (float)new_w / original_w;
    scale_factor_h = (float)new_h / original_h;

    /* Iniciamos la ventana */
    {
        std::string title = config["window"]["title"];
        window.init(fullscreen, new_w, new_h, title.c_str());
    }

    /* Iniciamos el renderer */
    {
        bool vsync = config["renderer"]["vsync"];
        renderer.init(vsync, scale_factor_w, scale_factor_h);
    }
}

//-----------------------------------------------------------------------------
// Contextos

void Client::_launchHomeCtx() {
    fprintf(stderr, "Lanzando home_context\n");
    current_context = GAME_CTX;
}

void Client::_launchGameCtx() {
    fprintf(stderr, "Lanzando game_context\n");
    current_context = EXIT_CTX;
}

//-----------------------------------------------------------------------------

void Client::_quitSDL() {
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Client::Client() : renderer(window), current_context(HOME_CTX) {
    _initSDL();
    _initComponents();
}

void Client::launch() {
    // Iniciamos la ejecución launcheando HOME_CTX
    _launchHomeCtx();

    while (current_context != EXIT_CTX) {
        switch (current_context) {
            case HOME_CTX: {
                _launchHomeCtx();
                break;
            }

            case GAME_CTX: {
                _launchGameCtx();
                break;
            }

            case EXIT_CTX: {
                break;
            }

            default: {
                throw Exception("Client::launch: unknown context.");
            }
        }
    }

    // salir ordenadamente
}

Client::~Client() {
    _quitSDL();
}

//-----------------------------------------------------------------------------
