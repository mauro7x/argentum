#include "../../../includes/Model/Commands/SendPMCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

SendPMCommand::SendPMCommand() : Command() {}

bool SendPMCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    return true;
}

SendPMCommand::~SendPMCommand() {}

//-----------------------------------------------------------------------------
