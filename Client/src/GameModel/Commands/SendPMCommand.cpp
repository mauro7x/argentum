#include "../../../includes/GameModel/Commands/SendPMCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

SendPMCommand::SendPMCommand(const std::string& receiver,
                             const std::string& content)
    : Command(), receiver(receiver), content(content) {}

bool SendPMCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)SEND_PRIVATE_MESSAGE_CMD)) {
        return false;
    }

    if (!(socket << receiver)) {
        return false;
    }

    if (!(socket << content)) {
        return false;
    }

    return true;
}

SendPMCommand::~SendPMCommand() {}

//-----------------------------------------------------------------------------
