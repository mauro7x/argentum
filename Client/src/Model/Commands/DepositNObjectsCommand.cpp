#include "../../../includes/Model/Commands/DepositNObjectsCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

DepositNObjectsCommand::DepositNObjectsCommand() : Command() {}

bool DepositNObjectsCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    return true;
}

DepositNObjectsCommand::~DepositNObjectsCommand() {}

//-----------------------------------------------------------------------------
