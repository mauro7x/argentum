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
    commands.emplace(DEPOSIT_GOLD_EXPECTED_INPUT, DEPOSIT_GOLD_INPUT_CMD);
    commands.emplace(WITHDRAW_EXPECTED_INPUT, WITHDRAW_INPUT_CMD);
    commands.emplace(WITHDRAW_GOLD_EXPECTED_INPUT, WITHDRAW_GOLD_INPUT_CMD);
    commands.emplace(BUY_EXPECTED_INPUT, BUY_INPUT_CMD);
    commands.emplace(SELL_EXPECTED_INPUT, SELL_INPUT_CMD);
    commands.emplace(LIST_PLAYERS_EXPECTED_INPUT, LIST_PLAYERS_INPUT_CMD);
    commands.emplace(HELP_NPC_INPUT, HELP_NPC_INPUT_CMD);
}

Command* InputParser::_parseCommandInput(
    const std::string& command_input) const {
    std::string identifier = _getCommandIdentifier(command_input);
    if (commands.count(identifier) == 0) {
        (*g_reply) = "Comando inexistente.";
        return NULL;
    }

    return _parseCommand(command_input, commands.at(identifier));
}

Command* InputParser::_parseCommand(const std::string& command_input,
                                    InputCommand cmd_id) const {
    std::string body;

    switch (cmd_id) {
        case EQUIP_INPUT_CMD: {
            (*g_reply) = "Este comando aun no está implementado.";
            return NULL;
        }

        case GRAB_INPUT_CMD: {
            body = _getCommandBody(command_input);
            if (body.empty()) {
                return new GrabItemCommand;
            }

            (*g_reply) = "El comando '/tomar' no admite parámetros.";
            return NULL;
        }

        case THROW_INPUT_CMD: {
            if (current_selection.inventory_slot_selected < 0) {
                (*g_reply) =
                    "Debes seleccionar el item que quieres tirar (haciéndole "
                    "click).";
                return NULL;
            }

            body = _getCommandBody(command_input);
            if (body.empty()) {
                return new ThrowNObjectsCommand(
                    current_selection.inventory_slot_selected, 1);
            }

            std::string n, excess;
            _splitBy(' ', body, n, excess);

            if (!excess.empty()) {
                (*g_reply) =
                    "Uso esperado del comando: /tirar [<cantidad> = 1]";
                return NULL;
            }

            uint32_t amount;
            if (!_castToUint32(n, amount)) {
                (*g_reply) =
                    "Uso esperado del comando: /tirar [<cantidad> = 1]";
                return NULL;
            }

            return new ThrowNObjectsCommand(
                current_selection.inventory_slot_selected, amount);
        }

        case MEDITATE_INPUT_CMD: {
            body = _getCommandBody(command_input);
            if (body.empty()) {
                return new MeditateCommand;
            }

            (*g_reply) = "El comando '/meditar' no admite parámetros.";
            return NULL;
        }

        case RESURRECT_INPUT_CMD: {
            body = _getCommandBody(command_input);
            if (!body.empty()) {
                (*g_reply) = "El comando '/resucitar' no admite parámetros.";
                return NULL;
            }

            if (current_selection.npc_selected) {
                return new ResurrectCommand(current_selection.npc_x_tile,
                                            current_selection.npc_y_tile);
            } else {
                return new SelfResurrectCommand();
            }
        }

        case HEAL_INPUT_CMD: {
            if (!current_selection.npc_selected) {
                (*g_reply) =
                    "Debes seleccionar a un sacerdote primero (haciéndole "
                    "click).";
                return NULL;
            }

            body = _getCommandBody(command_input);
            if (!body.empty()) {
                (*g_reply) = "El comando '/curar' no admite parámetros.";
                return NULL;
            }

            return new HealCommand(current_selection.npc_x_tile,
                                   current_selection.npc_y_tile);
        }

        case LIST_INPUT_CMD: {
            if (!current_selection.npc_selected) {
                (*g_reply) =
                    "Debes seleccionar a un NPC primero (haciéndole click).";
                return NULL;
            }

            body = _getCommandBody(command_input);
            if (!body.empty()) {
                (*g_reply) = "El comando '/listar' no admite parámetros.";
                return NULL;
            }

            return new ListCommand(current_selection.npc_x_tile,
                                   current_selection.npc_y_tile);
        }

        case DEPOSIT_INPUT_CMD: {
            if (!current_selection.npc_selected) {
                (*g_reply) =
                    "Debes seleccionar a un banquero primero (haciéndole "
                    "click).";
                return NULL;
            }

            if (current_selection.inventory_slot_selected < 0) {
                (*g_reply) =
                    "Debes seleccionar el item que quieres depositar "
                    "(haciéndole click).";
                return NULL;
            }

            body = _getCommandBody(command_input);
            if (body.empty()) {
                return new DepositNObjectsCommand(
                    current_selection.npc_x_tile, current_selection.npc_y_tile,
                    current_selection.inventory_slot_selected, 1);
            }

            std::string n, excess;
            _splitBy(' ', body, n, excess);

            if (!excess.empty()) {
                (*g_reply) =
                    "Uso esperado del comando: /depositar [<cantidad> = 1]";
                return NULL;
            }

            uint32_t amount;
            if (!_castToUint32(n, amount)) {
                (*g_reply) =
                    "Uso esperado del comando: /depositar [<cantidad> = 1]";
                return NULL;
            }

            return new DepositNObjectsCommand(
                current_selection.npc_x_tile, current_selection.npc_y_tile,
                current_selection.inventory_slot_selected, amount);
        }

        case DEPOSIT_GOLD_INPUT_CMD: {
            if (!current_selection.npc_selected) {
                (*g_reply) =
                    "Debes seleccionar a un banquero primero (haciéndole "
                    "click).";
                return NULL;
            }

            body = _getCommandBody(command_input);
            if (body.empty()) {
                (*g_reply) =
                    "Uso esperado del comando: /depositaroro <cantidad>";
                return NULL;
            }

            std::string n, excess;
            _splitBy(' ', body, n, excess);

            if (!excess.empty()) {
                (*g_reply) =
                    "Uso esperado del comando: /depositaroro <cantidad>";
                return NULL;
            }

            uint32_t amount;
            if (!_castToUint32(n, amount)) {
                (*g_reply) =
                    "Uso esperado del comando: /depositaroro <cantidad>";
                return NULL;
            }

            return new DepositGoldCommand(current_selection.npc_x_tile,
                                          current_selection.npc_y_tile, amount);
        }

        case WITHDRAW_INPUT_CMD: {
            if (!current_selection.npc_selected) {
                (*g_reply) =
                    "Debes seleccionar a un banquero primero (haciéndole "
                    "click).";
                return NULL;
            }

            body = _getCommandBody(command_input);
            if (body.empty()) {
                (*g_reply) =
                    "Uso esperado del comando: /retirar <item_id> [<cantidad> "
                    "= 1]";
                return NULL;
            }

            std::string s_id, s_amount, excess;
            _splitBy(' ', body, s_id, excess);
            _splitBy(' ', excess, s_amount, excess);

            if (s_id.empty() || s_amount.empty() || !excess.empty()) {
                (*g_reply) =
                    "Uso esperado del comando: /retirar <item_id> [<cantidad> "
                    "= 1]";
                return NULL;
            }

            uint32_t id, amount;
            if (!_castToUint32(s_id, id) || !_castToUint32(s_amount, amount)) {
                (*g_reply) =
                    "Uso esperado del comando: /retirar <item_id> [<cantidad> "
                    "= 1]";
                return NULL;
            }

            return new WithdrawNObjectsCommand(current_selection.npc_x_tile,
                                               current_selection.npc_y_tile, id,
                                               amount);
        }

        case WITHDRAW_GOLD_INPUT_CMD: {
            if (!current_selection.npc_selected) {
                (*g_reply) =
                    "Debes seleccionar a un banquero primero (haciéndole "
                    "click).";
                return NULL;
            }

            body = _getCommandBody(command_input);
            if (body.empty()) {
                (*g_reply) = "Uso esperado del comando: /retiraroro <cantidad>";
                return NULL;
            }

            std::string n, excess;
            _splitBy(' ', body, n, excess);

            if (!excess.empty()) {
                (*g_reply) = "Uso esperado del comando: /retiraroro <cantidad>";
                return NULL;
            }

            uint32_t amount;
            if (!_castToUint32(n, amount)) {
                (*g_reply) = "Uso esperado del comando: /retiraroro <cantidad>";
                return NULL;
            }

            return new WithdrawGoldCommand(current_selection.npc_x_tile,
                                           current_selection.npc_y_tile,
                                           amount);
        }

        case BUY_INPUT_CMD: {
            if (!current_selection.npc_selected) {
                (*g_reply) =
                    "Debes seleccionar a un comerciante primero (haciéndole "
                    "click).";
                return NULL;
            }

            body = _getCommandBody(command_input);
            if (body.empty()) {
                (*g_reply) =
                    "Uso esperado del comando: /comprar <item_id> [<cantidad> "
                    "= 1]";
                return NULL;
            }

            std::string s_id, s_amount, excess;
            _splitBy(' ', body, s_id, excess);
            _splitBy(' ', excess, s_amount, excess);

            uint32_t id;
            if (s_id.empty() || !excess.empty() || !_castToUint32(s_id, id)) {
                (*g_reply) =
                    "Uso esperado del comando: /comprar <item_id> [<cantidad> "
                    "= 1]";
                return NULL;
            }

            if (s_amount.empty()) {
                return new BuyNObjectsCommand(current_selection.npc_x_tile,
                                              current_selection.npc_y_tile, id,
                                              1);
            }

            uint32_t amount;
            if (!_castToUint32(s_amount, amount)) {
                (*g_reply) =
                    "Uso esperado del comando: /comprar <item_id> [<cantidad> "
                    "= 1]";
                return NULL;
            }

            return new BuyNObjectsCommand(current_selection.npc_x_tile,
                                          current_selection.npc_y_tile, id,
                                          amount);
        }

        case SELL_INPUT_CMD: {
            if (!current_selection.npc_selected) {
                (*g_reply) =
                    "Debes seleccionar a un comerciante primero (haciéndole "
                    "click).";
                return NULL;
            }

            if (current_selection.inventory_slot_selected < 0) {
                (*g_reply) =
                    "Debes seleccionar el item que quieres vender "
                    "(haciéndole click).";
                return NULL;
            }

            body = _getCommandBody(command_input);
            if (body.empty()) {
                return new SellNObjectsCommand(
                    current_selection.npc_x_tile, current_selection.npc_y_tile,
                    current_selection.inventory_slot_selected, 1);
            }

            std::string n, excess;
            _splitBy(' ', body, n, excess);

            if (!excess.empty()) {
                (*g_reply) =
                    "Uso esperado del comando: /vender [<cantidad> = 1]";
                return NULL;
            }

            uint32_t amount;
            if (!_castToUint32(n, amount)) {
                (*g_reply) =
                    "Uso esperado del comando: /vender [<cantidad> = 1]";
                return NULL;
            }

            return new SellNObjectsCommand(
                current_selection.npc_x_tile, current_selection.npc_y_tile,
                current_selection.inventory_slot_selected, amount);
        }

        case LIST_PLAYERS_INPUT_CMD: {
            body = _getCommandBody(command_input);
            if (!body.empty()) {
                (*g_reply) =
                    "El comando '/listarjugadores' no admite parámetros.";
                return NULL;
            }

            return new ListConnectedPlayersCommand();
        }

        case HELP_NPC_INPUT_CMD: {
            if (!current_selection.npc_selected) {
                (*g_reply) =
                    "Debes seleccionar al NPC al que le quieras pedir ayuda "
                    "(haciéndole "
                    "click).";
                return NULL;
            }

            body = _getCommandBody(command_input);
            if (!body.empty()) {
                (*g_reply) = "El comando '/help' no admite parámetros.";
                return NULL;
            }

            fprintf(stderr, "Envío comando npchelp con x=%u, y=%u\n",
                    current_selection.npc_x_tile, current_selection.npc_y_tile);
            return new HelpNpcCommand(current_selection.npc_x_tile,
                                      current_selection.npc_y_tile);
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
        return command_input.substr(index + 1);
    }
}

void InputParser::_splitBy(char delim, const std::string& original,
                           std::string& left, std::string& right) const {
    size_t index = original.find_first_of(delim);
    if (index == std::string::npos) {
        left = original;
        right = "";
    } else {
        left = original.substr(0, index);
        right = original.substr(index + 1);
    }
}

void InputParser::_splitBy(const std::string& delim,
                           const std::string& original, std::string& left,
                           std::string& right) const {
    size_t index = original.find(delim);
    if (index == std::string::npos) {
        left = original;
        right = "";
    } else {
        left = original.substr(0, index);
        right = original.substr(index + delim.size());
    }
}

Command* InputParser::_parsePMInput(const std::string& message_input) const {
    std::string receiver, content;
    _splitBy(": ", message_input.substr(1), receiver, content);

    if (receiver.empty() || content.empty()) {
        (*g_reply) =
            "Formato esperado para los mensajes privados: @destinatario: "
            "<mensaje>";
        return NULL;
    }

    fprintf(stderr, "Enviando mensaje [%s] a [%s]\n", content.c_str(),
            receiver.c_str());
    return new SendPMCommand(receiver, content);
}

bool InputParser::_isNumeric(const std::string& string) const {
    // Función sacada de:
    // https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
    return (!string.empty() &&
            std::find_if(string.begin(), string.end(), [](unsigned char c) {
                return !std::isdigit(c);
            }) == string.end());
}

bool InputParser::_castToUint32(const std::string& string,
                                uint32_t& result) const {
    uint32_t tmp;
    try {
        if (!_isNumeric(string)) {
            return false;
        }

        tmp = std::stoi(string);
    } catch (const std::exception& e) {
        return false;
    }

    result = tmp;
    return true;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

InputParser::InputParser(Selection& current_selection)
    : current_selection(current_selection) {
    _fillCommands();
}

Command* InputParser::parse(const std::string& input, std::string& reply) {
    // Setteamos el puntero a la respuesta
    g_reply = &reply;

    if (input.empty()) {
        return NULL;
    }

    char type = input.at(0);
    switch (type) {
        case '/': {
            return _parseCommandInput(input);
        }

        case '@': {
            return _parsePMInput(input);
        }

        default: {
            fprintf(stderr, "Enviando mensaje general [%s]\n", input.c_str());
            return new SendGeneralMessageCommand(input);
        }
    }
}

InputParser::~InputParser() {}

//-----------------------------------------------------------------------------
