#include "../../../includes/Model/Commands/WithdrawNObjectsCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

WithdrawNObjectsCommand::WithdrawNObjectsCommand() : Command() {}

bool WithdrawNObjectsCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    return true;
}

WithdrawNObjectsCommand::~WithdrawNObjectsCommand() {}

//-----------------------------------------------------------------------------
