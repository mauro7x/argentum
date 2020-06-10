#include "../../includes/Model/ServerProxy.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ServerProxy::ServerProxy(Queue<int*>& requests, Queue<PlayerData*>& broadcast)
    : requests(requests),
      broadcast(broadcast),
      player({0, 0, DOWN, 100, 100, 2000, 2100, 1300, 1400, 1500, 1000}),
      is_running(false),
      keep_running(true) {}

void ServerProxy::run() {
    is_running = true;
    int* request = NULL;
    PlayerData* broadcast = NULL;
    bool broadcastear = false;

    /* velocidad */
    bool moving = false;
    bool set_delta = false;
    Uint32 time = 0;
    Uint32 last_moved = 0;
    Uint32 delta = 0;
    Uint32 start = 0, stop = 0, sleep_for = 0;

    int x_step = 0, y_step = 0;
    start = SDL_GetTicks();

    while (keep_running) {
        broadcastear = false;

        /* procesar comandos */
        while ((request = requests.pop())) {
            fprintf(stderr, "Soy el server. Recibimos: %i.\n", *request);

            switch (*request) {
                case 0:
                    x_step = 0;
                    y_step = -1;
                    player.orientation = UP;
                    moving = true;
                    break;

                case 1:
                    x_step = 0;
                    y_step = 1;
                    player.orientation = DOWN;
                    moving = true;
                    break;

                case 2:
                    x_step = -1;
                    y_step = 0;
                    player.orientation = LEFT;
                    moving = true;
                    break;

                case 3:
                    x_step = 1;
                    y_step = 0;
                    player.orientation = RIGHT;
                    moving = true;
                    break;

                case 4:
                    x_step = 0;
                    y_step = 0;
                    moving = false;
                    set_delta = false;
                    break;
            }

            delete request;
        }

        time = SDL_GetTicks();

        if (moving) {
        } else {
        }

        if ((moving) && ((time - last_moved + delta) >= TIME_TO_MOVE_A_TILE)) {
            player.x_tile += x_step;
            player.y_tile += y_step;
            if (set_delta) {
                delta = (time - last_moved + delta) - TIME_TO_MOVE_A_TILE;
            } else {
                delta = 0;
            }

            last_moved = SDL_GetTicks();
            broadcastear = true;
        }

        // broadcastear
        if (broadcastear) {
            broadcast = new PlayerData(player);
            fprintf(stderr, "Soy el server. Voy a enviar un update.\n");
            this->broadcast.push(broadcast);
        }

        // control del frame rate
        stop = SDL_GetTicks();
        sleep_for = (start - stop) % (1000 / TPS);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_for));

        start += (1000 / TPS);
    }

    is_running = false;
}

bool ServerProxy::isRunning() {
    return is_running;
}

void ServerProxy::kill() {
    keep_running = false;
}

ServerProxy::~ServerProxy() {}

//-----------------------------------------------------------------------------
