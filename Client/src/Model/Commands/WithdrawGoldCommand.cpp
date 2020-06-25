#include "../../../includes/Model/Commands/WithdrawGoldCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

WithdrawGoldCommand::WithdrawGoldCommand() : Command() {}

bool WithdrawGoldCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    return true;
}

WithdrawGoldCommand::~WithdrawGoldCommand() {}

//-----------------------------------------------------------------------------
