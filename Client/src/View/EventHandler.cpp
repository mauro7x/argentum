#include "../../includes/View/EventHandler.h"

//-----------------------------------------------------------------------------
// Métodos privados

void EventHandler::_bindKeycodes() {
    // En un futuro se podría permitir al usuario bindear las teclas que quiera
    // utilizando esta función y algún configurable.

    keys.emplace(SDLK_w, UP_KEY);
    keys.emplace(SDLK_s, DOWN_KEY);
    keys.emplace(SDLK_a, LEFT_KEY);
    keys.emplace(SDLK_d, RIGHT_KEY);
    keys.emplace(SDLK_RETURN, ENTER_KEY);
    keys.emplace(SDLK_BACKSPACE, DELETE_KEY);
    keys.emplace(SDLK_ESCAPE, ESC_KEY);
}

Key EventHandler::_getKey(const SDL_Keycode& key) {
    if (keys.count(key)) {
        return keys.at(key);
    }

    return UNMAPPED_KEY;
}

Event EventHandler::_getEvent(const SDL_Event& e) {
    if (e.type == SDL_QUIT) {
        return EXIT_EV;
    }

    if ((e.type == SDL_TEXTINPUT) && (text_input_enabled)) {
        return TEXT_INPUT_EV;
    }

    if ((e.type == SDL_KEYDOWN) && (!text_input_enabled)) {
        Key key = _getKey(e.key.keysym.sym);

        switch (key) {
            case UNMAPPED_KEY: {
                break;
            }

            case UP_KEY: {
                if (!key_pressed) {
                    key_pressed = UP_KEY;
                    return START_MOVING_UP_EV;
                }
                break;
            }

            case DOWN_KEY: {
                if (!key_pressed) {
                    key_pressed = DOWN_KEY;
                    return START_MOVING_DOWN_EV;
                }
                break;
            }

            case LEFT_KEY: {
                if (!key_pressed) {
                    key_pressed = LEFT_KEY;
                    return START_MOVING_LEFT_EV;
                }
                break;
            }

            case RIGHT_KEY: {
                if (!key_pressed) {
                    key_pressed = RIGHT_KEY;
                    return START_MOVING_RIGHT_EV;
                }
                break;
            }

            case ENTER_KEY: {
                text_input_enabled = true;
                return START_INPUT_EV;
            }

            case ESC_KEY: {
                return EXIT_EV;
            }

            default: {
                break;
            }
        }
    }

    if ((e.type == SDL_KEYDOWN) && (text_input_enabled)) {
        Key key = _getKey(e.key.keysym.sym);

        switch (key) {
            case ENTER_KEY: {
                text_input_enabled = false;
                return STOP_INPUT_EV;
            }

            case DELETE_KEY: {
                return DELETE_CHAR_EV;
            }

            case ESC_KEY: {
                return EXIT_EV;
            }

            default: {
                break;
            }
        }
    }

    if (e.type == SDL_KEYUP && e.key.repeat == 0) {
        Key key = _getKey(e.key.keysym.sym);

        switch (key) {
            case UNMAPPED_KEY: {
                break;
            }

            case UP_KEY: {
                if (key_pressed == UP_KEY) {
                    key_pressed = UNMAPPED_KEY;
                    return STOP_MOVING_EV;
                }
                break;
            }

            case DOWN_KEY: {
                if (key_pressed == DOWN_KEY) {
                    key_pressed = UNMAPPED_KEY;
                    return STOP_MOVING_EV;
                }
                break;
            }

            case LEFT_KEY: {
                if (key_pressed == LEFT_KEY) {
                    key_pressed = UNMAPPED_KEY;
                    return STOP_MOVING_EV;
                }
                break;
            }

            case RIGHT_KEY: {
                if (key_pressed == RIGHT_KEY) {
                    key_pressed = UNMAPPED_KEY;
                    return STOP_MOVING_EV;
                }
                break;
            }

            default: {
                break;
            }
        }
    }

    return INVALID_EV;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

EventHandler::EventHandler(std::atomic_bool& exit, HUD& hud,
                           BlockingQueue<Command*>& commands)
    : exit(exit), hud(hud), commands(commands) {
    _bindKeycodes();
}

void EventHandler::handleEvent(const SDL_Event& e) {
    Event event = _getEvent(e);

    switch (event) {
        // Eventos de SDL que no nos interesan
        case INVALID_EV: {
            break;
        }

        // Control
        case EXIT_EV: {
            exit = true;
            break;
        }

        // Movimiento
        case START_MOVING_UP_EV: {
            commands.push(new StartMovingCommand(UP_DIR));
            break;
        }

        case START_MOVING_DOWN_EV: {
            commands.push(new StartMovingCommand(DOWN_DIR));
            break;
        }

        case START_MOVING_LEFT_EV: {
            commands.push(new StartMovingCommand(LEFT_DIR));
            break;
        }

        case START_MOVING_RIGHT_EV: {
            commands.push(new StartMovingCommand(RIGHT_DIR));
            break;
        }

        case STOP_MOVING_EV: {
            commands.push(new StopMovingCommand());
            break;
        }

        case START_INPUT_EV: {
            hud.enableInput();
            break;
        }

        case TEXT_INPUT_EV: {
            hud.newInputText(e.text.text);
            break;
        }

        case DELETE_CHAR_EV: {
            hud.removeChar();
            break;
        }

        case STOP_INPUT_EV: {
            std::string input = hud.popText();

            Command* cmd = input_parser.parse(input);
            if (cmd) {
                fprintf(stderr, "Enviando comando al servidor.\n");
                commands.push(cmd);
            } else if (!input.empty()) {
                fprintf(stderr, "No se reconoció el comando: %s\n",
                        input.c_str());
            }

            hud.disableInput();
            break;
        }

        // No deberíamos llegar acá nunca
        default: {
            throw Exception(
                "EventHandler::handleEvent: unknown event received.");
        }
    }
}

EventHandler::~EventHandler() {}

//-----------------------------------------------------------------------------
