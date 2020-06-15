#include "../../includes/View/EventHandler.h"

//-----------------------------------------------------------------------------
// Métodos privados

void EventHandler::_bindKeycodes() {
    keys.emplace(SDLK_w, UP_KEY);
    keys.emplace(SDLK_s, DOWN_KEY);
    keys.emplace(SDLK_a, LEFT_KEY);
    keys.emplace(SDLK_d, RIGHT_KEY);
    keys.emplace(SDLK_ESCAPE, ESC_KEY);
}

Key EventHandler::_getKey(const SDL_Keycode& key) {
    if (keys.count(key)) {
        return keys.at(key);
    }

    return UNMAPPED_KEY;
}

Event EventHandler::_getEvent(const SDL_Event& e) {
    Key key;

    if (e.type == SDL_QUIT) {
        return EXIT;
    }

    if (e.type == SDL_KEYDOWN /*&& e.key.repeat == 0*/) {
        key = _getKey(e.key.keysym.sym);

        switch (key) {
            case UNMAPPED_KEY: {
                break;
            }

            case UP_KEY: {
                if (!key_pressed) {
                    key_pressed = UP_KEY;
                    return START_MOVING_UP;
                }
                break;
            }

            case DOWN_KEY: {
                if (!key_pressed) {
                    key_pressed = DOWN_KEY;
                    return START_MOVING_DOWN;
                }
                break;
            }

            case LEFT_KEY: {
                if (!key_pressed) {
                    key_pressed = LEFT_KEY;
                    return START_MOVING_LEFT;
                }
                break;
            }

            case RIGHT_KEY: {
                if (!key_pressed) {
                    key_pressed = RIGHT_KEY;
                    return START_MOVING_RIGHT;
                }
                break;
            }

            case ESC_KEY: {
                return EXIT;
            }

            default: {
                break;
            }
        }
    }

    if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        key = _getKey(e.key.keysym.sym);

        switch (key) {
            case UNMAPPED_KEY: {
                break;
            }

            case UP_KEY: {
                if (key_pressed == UP_KEY) {
                    key_pressed = UNMAPPED_KEY;
                    return STOP_MOVING;
                }
                break;
            }

            case DOWN_KEY: {
                if (key_pressed == DOWN_KEY) {
                    key_pressed = UNMAPPED_KEY;
                    return STOP_MOVING;
                }
                break;
            }

            case LEFT_KEY: {
                if (key_pressed == LEFT_KEY) {
                    key_pressed = UNMAPPED_KEY;
                    return STOP_MOVING;
                }
                break;
            }

            case RIGHT_KEY: {
                if (key_pressed == RIGHT_KEY) {
                    key_pressed = UNMAPPED_KEY;
                    return STOP_MOVING;
                }
                break;
            }

            default: {
                break;
            }
        }
    }

    return INVALID;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

EventHandler::EventHandler(std::atomic_bool& view_running,
                           Queue<int*>& requests)
    : view_running(view_running),
      requests(requests),
      key_pressed(UNMAPPED_KEY) {
    _bindKeycodes();
}

void EventHandler::run() {
    SDL_Event e;
    Event event;

    /* Proxy */
    int* cmd = NULL;

    while (view_running) {
        while (SDL_PollEvent(&e) != 0) {
            event = _getEvent(e);

            switch (event) {
                case INVALID: {
                    break;
                }

                case EXIT: {
                    view_running = false;
                    break;
                }

                case START_MOVING_UP: {
                    /* proxy */
                    cmd = new int(0);
                    fprintf(
                        stderr,
                        "EVENT-HANDLER | START_MOVING_UP | Enviamos un %i.\n",
                        *cmd);
                    requests.push(cmd);

                    break;
                }

                case START_MOVING_DOWN: {
                    /* proxy */
                    cmd = new int(1);
                    fprintf(
                        stderr,
                        "EVENT-HANDLER | START_MOVING_DOWN | Enviamos un %i.\n",
                        *cmd);
                    requests.push(cmd);

                    break;
                }

                case START_MOVING_LEFT: {
                    cmd = new int(2);
                    fprintf(
                        stderr,
                        "EVENT-HANDLER | START_MOVING_LEFT | Enviamos un %i.\n",
                        *cmd);
                    requests.push(cmd);

                    break;
                }

                case START_MOVING_RIGHT: {
                    cmd = new int(3);
                    fprintf(stderr,
                            "EVENT-HANDLER | START_MOVING_RIGHT | Enviamos un "
                            "%i.\n",
                            *cmd);
                    requests.push(cmd);

                    break;
                }

                case STOP_MOVING: {
                    cmd = new int(4);
                    fprintf(stderr,
                            "EVENT-HANDLER | STOP_MOVING | Enviamos un %i.\n",
                            *cmd);
                    requests.push(cmd);

                    break;
                }

                default: {
                    break;
                }
            }
        }
    }
}

EventHandler::~EventHandler() {}

//-----------------------------------------------------------------------------
