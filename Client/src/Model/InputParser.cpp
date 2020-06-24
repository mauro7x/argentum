#include "../../includes/Model/InputParser.h"

//-----------------------------------------------------------------------------
// Métodos privados

void InputParser::_fillCommands() {
    commands.emplace(EQUIP_EXPECTED_INPUT, EQUIP_INPUT_CMD);
    commands.emplace(GRAB_EXPECTED_INPUT, GRAB_INPUT_CMD);
    commands.emplace(THROW_EXPECTED_INPUT, THROW_INPUT_CMD);
    commands.emplace(MEDITATE_EXPECTED_INPUT, MEDITATE_INPUT_CMD);
    commands.emplace(RESURRECT_EXPECTED_INPUT, RESURRECT_INPUT_CMD);
    commands.emplace(HEAL_EXPECTED_INPUT, HEAL_INPUT_CMD);
    commands.emplace(LIST_EXPECTED_INPUT, LIST_INPUT_CMD);
    commands.emplace(DEPOSIT_EXPECTED_INPUT, DEPOSIT_INPUT_CMD);
    commands.emplace(WITHDRAW_EXPECTED_INPUT, WITHDRAW_INPUT_CMD);
    commands.emplace(BUY_EXPECTED_INPUT, BUY_INPUT_CMD);
    commands.emplace(SELL_EXPECTED_INPUT, SELL_INPUT_CMD);
    commands.emplace(LIST_PLAYERS_EXPECTED_INPUT, LIST_PLAYERS_INPUT_CMD);
}

Command* InputParser::_parseCommandInput(
    const std::string& command_input) const {
    std::string identifier = _getCommandIdentifier(command_input);
    if (commands.count(identifier) == 0) {
        return NULL;
    }

    return _parseCommand(command_input, commands.at(identifier));
}

Command* InputParser::_parseCommand(const std::string& command_input,
                                    InputCommand cmd_id) const {
    std::string body;

    switch (cmd_id) {
        case EQUIP_INPUT_CMD: {
            fprintf(stderr,
                    "InputParser::_parseCommand: comando EQUIPAR aun no "
                    "implementado.\n");
            return NULL;
        }

        case GRAB_INPUT_CMD: {
            body = _getCommandBody(command_input);
            if (body.empty()) {
                return new GrabItemCommand;
            }

            return NULL;
        }

        case THROW_INPUT_CMD: {
            fprintf(stderr,
                    "InputParser::_parseCommand: comando TIRAR aun no "
                    "implementado.\n");
            return NULL;
        }

        case MEDITATE_INPUT_CMD: {
            body = _getCommandBody(command_input);
            if (body.empty()) {
                return new MeditateCommand;
            }

            return NULL;
        }

        case RESURRECT_INPUT_CMD: {
            body = _getCommandBody(command_input);
            if (body.empty()) {
                return new SelfResurrectCommand;
            } else {
                fprintf(stderr,
                        "InputParser::_parseCommand: comando RESUCITAR CON "
                        "SACERDOTE aun no "
                        "implementado.\n");
                return NULL;
            }
        }

        case HEAL_INPUT_CMD: {
            fprintf(stderr,
                    "InputParser::_parseCommand: comando CURAR aun no "
                    "implementado.\n");
            return NULL;
        }

        case LIST_INPUT_CMD: {
            fprintf(stderr,
                    "InputParser::_parseCommand: comando LISTAR aun no "
                    "implementado.\n");
            return NULL;
        }

        case DEPOSIT_INPUT_CMD: {
            fprintf(stderr,
                    "InputParser::_parseCommand: comando DEPOSITAR aun no "
                    "implementado.\n");
            return NULL;
        }

        case WITHDRAW_INPUT_CMD: {
            fprintf(stderr,
                    "InputParser::_parseCommand: comando RETIRAR aun no "
                    "implementado.\n");
            return NULL;
        }

        case BUY_INPUT_CMD: {
            fprintf(stderr,
                    "InputParser::_parseCommand: comando COMPRAR aun no "
                    "implementado.\n");
            return NULL;
        }

        case SELL_INPUT_CMD: {
            fprintf(stderr,
                    "InputParser::_parseCommand: comando VENDER aun no "
                    "implementado.\n");
            return NULL;
        }

        case LIST_PLAYERS_INPUT_CMD: {
            fprintf(stderr,
                    "InputParser::_parseCommand: comando LISTAR JUGADORES "
                    "CONECTADOS aun no "
                    "implementado.\n");
            return NULL;
        }

        default: {
            throw Exception(
                "InputParser::_parseCommand: unknown cmd_id "
                "received.");
        }
    }
}

std::string InputParser::_getCommandIdentifier(
    const std::string& command_input) const {
    size_t index = command_input.find_first_of(' ');
    return command_input.substr(0, index);
}

std::string InputParser::_getCommandBody(
    const std::string& command_input) const {
    size_t index = command_input.find_first_of(' ');
    if (index == std::string::npos) {
        return std::string("");
    } else {
        return command_input.substr(index);
    }
}

Command* InputParser::_parseMessageInput(
    const std::string& message_input) const {
    fprintf(stderr,
            "InputParser::_parseMessageInput: mensajes privados todavía no "
            "implementados!\n");

    return NULL;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

InputParser::InputParser() {
    _fillCommands();
}

Command* InputParser::parse(const std::string& input) const {
    if (input.empty()) {
        return NULL;
    }

    char type = input.at(0);
    switch (type) {
        case '/': {
            return _parseCommandInput(input);
        }

        case '@': {
            return _parseMessageInput(input);
        }

        default: {
            return NULL;
        }
    }
}

InputParser::~InputParser() {}

//-----------------------------------------------------------------------------
