#include "../../includes/View/EventHandler.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

EventHandler::EventHandler(std::atomic_bool& view_running,
                           Queue<int*>& requests)
    : view_running(view_running), requests(requests) {}

void EventHandler::run() {
    SDL_Event e;
    int* cmd = NULL;

    while (view_running) {
        while (SDL_PollEvent(&e) != 0) {
            /* El cliente cerró el juego */
            if (e.type == SDL_QUIT) {
                view_running = false;
                break;
            }

            if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
                switch (e.key.keysym.sym) {
                    case SDLK_w:
                        cmd = new int(0);
                        fprintf(stderr, "Soy el cliente. Enviamos un %i.\n",
                                *cmd);
                        requests.push(cmd);
                        break;

                    case SDLK_s:
                        cmd = new int(1);
                        fprintf(stderr, "Soy el cliente. Enviamos un %i.\n",
                                *cmd);
                        requests.push(cmd);
                        break;

                    case SDLK_a:
                        cmd = new int(2);
                        fprintf(stderr, "Soy el cliente. Enviamos un %i.\n",
                                *cmd);
                        requests.push(cmd);
                        break;

                    case SDLK_d:
                        cmd = new int(3);
                        fprintf(stderr, "Soy el cliente. Enviamos un %i.\n",
                                *cmd);
                        requests.push(cmd);
                        break;
                }
            }

            if (e.type == SDL_KEYUP && e.key.repeat == 0) {
                switch (e.key.keysym.sym) {
                    case SDLK_w:
                    case SDLK_s:
                    case SDLK_a:
                    case SDLK_d:
                        cmd = new int(4);
                        fprintf(stderr, "Soy el cliente. Enviamos un %i.\n",
                                *cmd);
                        requests.push(cmd);
                        break;
                }
            }
        }
    }
}

EventHandler::~EventHandler() {}

//-----------------------------------------------------------------------------
