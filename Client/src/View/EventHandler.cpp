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
    switch (e.type) {
        case SDL_QUIT: {
            return EXIT_EV;
        }

        case SDL_TEXTINPUT: {
            return _getTextInputEv();
        }

        case SDL_MOUSEBUTTONDOWN: {
            return _getMouseButtonDownEv(e);
        }

        case SDL_KEYDOWN: {
            return _getKeyDownEv(e);
        }

        case SDL_KEYUP: {
            return _getKeyUpEv(e);
        }

        default: {
            return INVALID_EV;
        }
    }
}

// Traductores de eventos particulares

Event EventHandler::_getTextInputEv() {
    if (text_input_enabled) {
        return TEXT_INPUT_EV;
    }
    return INVALID_EV;
}

Event EventHandler::_getMouseButtonDownEv(const SDL_Event& e) {
    if (e.button.button != SDL_BUTTON_LEFT) {
        return INVALID_EV;
    }

    bool on_camera = _clickInside(camera_box, e);
    bool on_inventory = _clickInside(inventory_box, e);

    switch (e.button.clicks) {
        case 1: {
            // Single left click

            if (on_camera) {
                fprintf(stderr, "Click en la camara del juego!\n");
            } else if (on_inventory) {
                fprintf(stderr, "Click en el inventario!\n");
            }

            break;
        }

        case 2: {
            // Double left click

            if (on_inventory) {
                fprintf(stderr, "Doble click en el inventario!\n");
            }

            break;
        }

        default: {
            break;
        }
    }

    return INVALID_EV;
}

Event EventHandler::_getKeyDownEv(const SDL_Event& e) {
    Key key = _getKey(e.key.keysym.sym);

    // La mayoría de las veces la tecla no interesa
    if (key == UNMAPPED_KEY) {
        return INVALID_EV;
    }

    if (!text_input_enabled) {
        switch (key) {
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
    } else {
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

    return INVALID_EV;
}

Event EventHandler::_getKeyUpEv(const SDL_Event& e) {
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

    return INVALID_EV;
}

// Métodos auxiliares

bool EventHandler::_clickInside(const SDL_Rect& rect,
                                const SDL_Event& e) const {
    // Primero escalamos el click
    SDL_Point click_pos = {((int)(e.button.x / scale_factor_w)),
                           ((int)(e.button.y / scale_factor_h))};

    if ((click_pos.x > (rect.x + rect.w)) || (click_pos.x < (rect.x)) ||
        (click_pos.y > (rect.y + rect.h)) || (click_pos.y < (rect.y))) {
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

EventHandler::EventHandler(std::atomic_bool& exit,
                           BlockingQueue<Command*>& commands, HUD& hud,
                           const MapView& map)
    : exit(exit), commands(commands), hud(hud), map(map) {
    _bindKeycodes();
}

void EventHandler::init(const json& config, const float scale_factor_w,
                        const float scale_factor_h) {
    // Cargamos la box de la cámara del juego
    camera_box.x = config["camera"]["offset"]["x"];
    camera_box.y = config["camera"]["offset"]["y"];
    camera_box.w = config["camera"]["w"];
    camera_box.h = config["camera"]["h"];

    // Cargamos la box del inventario
    inventory_box.x = config["hud"]["user_inventory"]["offset"]["x"];
    inventory_box.y = config["hud"]["user_inventory"]["offset"]["y"];
    inventory_box.w = config["hud"]["user_inventory"]["w"];
    inventory_box.h = config["hud"]["user_inventory"]["h"];

    this->scale_factor_w = scale_factor_w;
    this->scale_factor_h = scale_factor_h;
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

        // Texto
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
                hud.addMessage(">> " + input, USER_CMD_MSG_COLOR);
                commands.push(cmd);
            } else if (!input.empty()) {
                hud.addMessage("Comando inexistente.", ERROR_MSG_COLOR);
            }

            hud.disableInput();
            break;
        }

        // Clicks
        case MAP_SINGLE_CLICK_EV: {
            fprintf(stderr, "Click en el mapa. Falta implementar.\n");
            break;
        }

        case HUD_SINGLE_CLICK_EV: {
            fprintf(stderr, "Click en el inventario. Falta implementar.\n");
            break;
        }

        case HUD_DOUBLE_CLICK_EV: {
            fprintf(stderr,
                    "Doble click en el inventario. Falta implementar.\n");
            break;
        }

        default: {
            throw Exception(
                "EventHandler::handleEvent: unknown event received.");
        }
    }
}

EventHandler::~EventHandler() {}

//-----------------------------------------------------------------------------
