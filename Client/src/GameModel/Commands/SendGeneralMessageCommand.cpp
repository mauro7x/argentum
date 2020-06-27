#include "../../../includes/GameModel/Commands/SendGeneralMessageCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

SendGeneralMessageCommand::SendGeneralMessageCommand(const std::string& content)
    : Command(), content(content) {}

bool SendGeneralMessageCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)SEND_GENERAL_MESSAGE_CMD)) {
        return false;
    }

    if (!(socket << content)) {
        return false;
    }

    return true;
}

SendGeneralMessageCommand::~SendGeneralMessageCommand() {}

//-----------------------------------------------------------------------------
