#include "../../../includes/Model/Commands/DepositGoldCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

DepositGoldCommand::DepositGoldCommand() : Command() {}

bool DepositGoldCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    return true;
}

DepositGoldCommand::~DepositGoldCommand() {}

//-----------------------------------------------------------------------------
