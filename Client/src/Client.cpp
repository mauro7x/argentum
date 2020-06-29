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
    fprintf(stderr, "Inicia HOME.\n");
    HomeView home_view(current_context, renderer, socket);
    home_view.run();
    fprintf(stderr, "Finaliza HOME.\n");
}

void Client::_launchConnectionCtx() {
    fprintf(stderr, "Inicia CONNECTION.\n");
    ConnectionView connection_view(current_context, renderer, socket);
    connection_view.run();
    fprintf(stderr, "Finaliza CONNECTION.\n");
}

void Client::_launchSignUpCtx() {
    fprintf(stderr, "Inicia SIGNUP.\n");
    SignUpView signup_view(current_context, renderer, socket);
    signup_view.run();
    fprintf(stderr, "Finaliza SIGNUP.\n");
}

void Client::_launchGameCtx() {
    fprintf(stderr, "Inicia GAME.\n");

    // Colas (thread-safe) de comunicación entre hilos
    BlockingQueue<Command*> commands;
    NonBlockingQueue<Broadcast*> broadcasts;
    NonBlockingQueue<Message*> messages;

    // Recibimos el primer paquete
    _receiveFirstPackage(broadcasts);

    // Componentes del GameCtx
    GameView game_view(commands, broadcasts, messages, renderer);
    CommandDispatcher command_dispatcher(socket, commands, game_view);
    Receiver receiver(socket, broadcasts, messages, game_view);

    // Lanzamos la ejecución
    command_dispatcher.start();
    receiver.start();

    try {
        game_view.run();
    } catch (const Exception& e) {
        _finishGameCtx(commands, command_dispatcher, receiver);
        throw e;
    } catch (const std::exception& e) {
        _finishGameCtx(commands, command_dispatcher, receiver);
        throw e;
    } catch (...) {
        _finishGameCtx(commands, command_dispatcher, receiver);
        throw;
    }

    // Finalizamos la ejecución
    _finishGameCtx(commands, command_dispatcher, receiver);

    // Luego de que el game termina, salimos
    current_context = EXIT_CTX;
    fprintf(stderr, "Finaliza GAME.\n");
}

//-----------------------------------------------------------------------------
// Auxiliares

void Client::_receiveFirstPackage(
    NonBlockingQueue<Broadcast*>& broadcasts) const {
    uint8_t opcode = 0, entity_type = 0;
    size_t received;

    // Recibimos el primer paquete según el procolo:
    // OP (1) - BROADCAST_OP (1) - ENTITY_TYPE (1) - LENGTH (4) - DATA (LENGTH)

    received = (socket >> opcode);
    if (!received) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: incomplete first package data "
            "(socket was closed).");
    } else if (opcode != BROADCAST_OPCODE) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: received another package before "
            "the initial data.");
    }

    received = (socket >> opcode);
    if (!received) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: incomplete first package data "
            "(socket was closed).");
    } else if (opcode != NEW_BROADCAST) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: received another package before "
            "the initial data.");
    }

    received = (socket >> entity_type);
    if (!received) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: incomplete first package data "
            "(socket was closed).");
    } else if (entity_type != PLAYER_TYPE) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: received another package before "
            "the initial data.");
    }

    std::vector<uint8_t> serialized_data;
    received = (socket >> serialized_data);
    if (!received) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: incomplete first package data "
            "(socket was closed).");
    }

    PlayerData received_data;
    json j = json::from_msgpack(serialized_data);
    received_data = j.get<PlayerData>();
    broadcasts.push(new NewPlayerBroadcast(received_data));
}

void Client::_finishGameCtx(BlockingQueue<Command*>& commands,
                            CommandDispatcher& command_dispatcher,
                            Receiver& receiver) {
    // Terminamos la ejecución
    commands.close();
    socket.shutdown();
    command_dispatcher.join();
    receiver.join();
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

            case CONNECTION_CTX: {
                _launchConnectionCtx();
                break;
            }

            case SIGNUP_CTX: {
                _launchSignUpCtx();
                break;
            }

            case GAME_CTX: {
                _launchGameCtx();
                break;
            }

            default: {
                throw Exception("Client::launch: unknown context.");
            }
        }
    }

    // Salir ordenadamente
    socket.shutdown();
}

Client::~Client() {
    _quitSDL();
}

//-----------------------------------------------------------------------------
