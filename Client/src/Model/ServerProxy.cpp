#include "../../includes/Model/ServerProxy.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ServerProxy::ServerProxy(NonBlockingQueue<int*>& requests,
                         NonBlockingQueue<PlayerData*>& broadcast)
    : requests(requests),
      broadcast(broadcast),
      player(
          {{1, 0, 0, DOWN_ORIENTATION},
           100,
           100,
           100,
           {InventorySlot({0, 0}), InventorySlot({0, 0}), InventorySlot({0, 0}),
            InventorySlot({0, 0}), InventorySlot({0, 0}), InventorySlot({0, 0}),
            InventorySlot({0, 0}), InventorySlot({0, 0}), InventorySlot({0, 0}),
            InventorySlot({0, 0})},
           {0, 0, 0, 0},
           2000,
           2100,
           1300,
           1400,
           1500,
           1000}),
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

    Uint32 t1 = SDL_GetTicks(), t2 = 0;

    int x_step = 0, y_step = 0;

    int it = 0;
    int start = 0, stop = 0, rest = 0, behind = 0, lost = 0;
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
                    player.basic_data.orientation = UP_ORIENTATION;
                    broadcastear = true;
                    moving = true;
                    break;

                case 1:
                    x_step = 0;
                    y_step = 1;
                    player.basic_data.orientation = DOWN_ORIENTATION;
                    broadcastear = true;
                    moving = true;
                    break;

                case 2:
                    x_step = -1;
                    y_step = 0;
                    player.basic_data.orientation = LEFT_ORIENTATION;
                    broadcastear = true;
                    moving = true;
                    break;

                case 3:
                    x_step = 1;
                    y_step = 0;
                    player.basic_data.orientation = RIGHT_ORIENTATION;
                    broadcastear = true;
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

        if ((moving) && ((time - last_moved + delta) >= TIME_TO_MOVE_A_TILE)) {
            if ((player.basic_data.x_tile + x_step) < 0 ||
                (player.basic_data.x_tile + x_step) == MAP_W_TILES ||
                (player.basic_data.y_tile + y_step) < 0 ||
                (player.basic_data.y_tile + y_step) == MAP_H_TILES) {
                /* avisar que no se puede mover xq hay colision */
                fprintf(stderr, "No se puede mover ahi\n");
                x_step = 0;
                y_step = 0;
                moving = false;
                set_delta = false;

            } else {
                player.basic_data.x_tile += x_step;
                player.basic_data.y_tile += y_step;
                if (set_delta) {
                    delta = (time - last_moved + delta) - TIME_TO_MOVE_A_TILE;
                } else {
                    delta = 0;
                }
                last_moved = SDL_GetTicks();

                broadcastear = true;
            }
        }

        // broadcastear
        if (broadcastear) {
            broadcast = new PlayerData(player);
            fprintf(stderr, "Soy el server. Voy a enviar un update.\n");
            t2 = SDL_GetTicks();
            fprintf(stderr, "Server. Envio paquete tras: %d ms\n", t2 - t1);
            t1 = SDL_GetTicks();
            this->broadcast.push(broadcast);
        }

        // control del frame rate
        stop = SDL_GetTicks();
        rest = PERIOD - (stop - start);

        if (rest < 0) {
            behind = -rest;
            lost = (behind - (behind % PERIOD));
            rest = PERIOD - (behind % PERIOD);
            start += lost;
            it += (int)(lost / PERIOD);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(rest));
        start += PERIOD;
        it += 1;
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
