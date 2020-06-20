#include "../../../includes/Model/Commands/StopMovingCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

StopMovingCommand::StopMovingCommand() : Command() {}

bool StopMovingCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    ssize_t sent = 0;

    sent = (socket << (char)COMMAND_OPCODE);
    if (!sent) {
        return false;
    }

    sent = (socket << (char)STOP_MOVING_CMD);
    if (!sent) {
        return false;
    }

    return true;
}

StopMovingCommand::~StopMovingCommand() {}

//-----------------------------------------------------------------------------
