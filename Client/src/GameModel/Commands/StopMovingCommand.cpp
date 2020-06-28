#include "../../../includes/GameModel/Commands/StopMovingCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

StopMovingCommand::StopMovingCommand() : Command() {}

bool StopMovingCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)STOP_MOVING_CMD)) {
        return false;
    }

    return true;
}

StopMovingCommand::~StopMovingCommand() {}

//-----------------------------------------------------------------------------
