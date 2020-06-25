#include "../../../includes/Model/Commands/ResurrectCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ResurrectCommand::ResurrectCommand() : Command() {}

bool ResurrectCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    return true;
}

ResurrectCommand::~ResurrectCommand() {}

//-----------------------------------------------------------------------------
