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

EventHandler::EventHandler(std::atomic_bool& exit,
                           BlockingQueue<Command*>& commands)
    : exit(exit), commands(commands), key_pressed(UNMAPPED_KEY) {
    _bindKeycodes();
}

void EventHandler::handleEvent(const SDL_Event& e) {
    Event event = _getEvent(e);

    switch (event) {
        // Eventos de SDL que no nos interesan
        case INVALID: {
            break;
        }

        // Control
        case EXIT: {
            exit = true;
            break;
        }

        // Movimiento
        case START_MOVING_UP: {
            commands.push(new StartMovingCommand(UP_DIR));
            break;
        }

        case START_MOVING_DOWN: {
            commands.push(new StartMovingCommand(DOWN_DIR));
            break;
        }

        case START_MOVING_LEFT: {
            commands.push(new StartMovingCommand(LEFT_DIR));
            break;
        }

        case START_MOVING_RIGHT: {
            commands.push(new StartMovingCommand(RIGHT_DIR));
            break;
        }

        case STOP_MOVING: {
            commands.push(new StopMovingCommand());
            break;
        }

        // Evento desconocido
        default: {
            break;
        }
    }
}

EventHandler::~EventHandler() {}

//-----------------------------------------------------------------------------
