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

void EventHandler::_clearSelection() {
    current_selection.npc_selected = false;
    current_selection.npc_x_tile = 0;
    current_selection.npc_y_tile = 0;
    current_selection.inventory_slot_selected = -1;
    hud.clearSelection();
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
    if (on_camera) {
        return CAMERA_SINGLE_CLICK_EV;
    }

    bool on_inventory = _clickInside(inventory_box, e);
    if (on_inventory) {
        if (e.button.clicks == 2) {
            return INVENTORY_DOUBLE_CLICK_EV;
        }

        return INVENTORY_SINGLE_CLICK_EV;
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

SDL_Point EventHandler::_getClickPos(const SDL_Event& e) const {
    return SDL_Point({((int)(e.button.x / scale_factor_w)),
                      ((int)(e.button.y / scale_factor_h))});
}

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

SDL_Point EventHandler::_clickToTile(const SDL_Event& e) const {
    SDL_Point click_pos = _getClickPos(e);
    click_pos.x -= camera.xOffset();
    click_pos.y -= camera.yOffset();

    return SDL_Point({click_pos.x / TILE_WIDTH, click_pos.y / TILE_HEIGHT});
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

EventHandler::EventHandler(std::atomic_bool& exit,
                           BlockingQueue<Command*>& commands, HUD& hud,
                           const MapView& map, const Camera& camera)
    : exit(exit),
      commands(commands),
      hud(hud),
      map(map),
      camera(camera),
      input_parser(current_selection) {
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
            _clearSelection();
            commands.push(new StartMovingCommand(UP_DIR));
            break;
        }

        case START_MOVING_DOWN_EV: {
            _clearSelection();
            commands.push(new StartMovingCommand(DOWN_DIR));
            break;
        }

        case START_MOVING_LEFT_EV: {
            _clearSelection();
            commands.push(new StartMovingCommand(LEFT_DIR));
            break;
        }

        case START_MOVING_RIGHT_EV: {
            _clearSelection();
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
            hud.disableInput();
            std::string input = hud.popText();
            std::string reply;
            Command* cmd = input_parser.parse(input, reply);
            if (cmd) {
                hud.addMessage(">> " + input, USER_CMD_MSG_COLOR);
                commands.push(cmd);
            } else if (!reply.empty()) {
                hud.addMessage(reply, WARNING_MSG_COLOR);
            }

            _clearSelection();
            break;
        }

        // Clicks
        case CAMERA_SINGLE_CLICK_EV: {
            // Obtenemos el tile clickeado
            SDL_Point tile = _clickToTile(e);

            // Si el tile tiene un ocupante, usamos el arma principal
            InstanceId occupant = map.getOccupant(tile.x, tile.y);
            if (occupant) {
                commands.push(new UseMainWeaponCommand(occupant));
                break;
            }

            // Si el tile tiene un NPC, lo seleccionamos para futuros comandos
            Id npc = map.getNPC(tile.x, tile.y);
            if (npc) {
                current_selection.npc_selected = true;
                current_selection.npc_x_tile = tile.x;
                current_selection.npc_y_tile = tile.y;
                hud.addMessage(
                    ">> Has seleccionado a un NPC, ahora puedes enviarle "
                    "comandos.",
                    USER_CMD_MSG_COLOR);
                break;
            }

            break;
        }

        case INVENTORY_SINGLE_CLICK_EV: {
            SDL_Point click_pos = _getClickPos(e);

            int8_t inventory_slot = hud.getInventorySlotClicked(click_pos);
            if (inventory_slot >= 0) {
                current_selection.inventory_slot_selected = inventory_slot;
                hud.selectItem(inventory_slot);
            }

            break;
        }

        case INVENTORY_DOUBLE_CLICK_EV: {
            _clearSelection();

            SDL_Point click_pos = _getClickPos(e);

            int8_t inventory_slot = hud.getInventorySlotClicked(click_pos);
            if (inventory_slot >= 0) {
                commands.push(new EquipObjectCommand(inventory_slot));
                break;
            }

            int8_t equipment_slot = hud.getEquipmentSlotClicked(click_pos);
            if (equipment_slot >= 0) {
                commands.push(new UnequipObjectCommand(equipment_slot));
                break;
            }

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
