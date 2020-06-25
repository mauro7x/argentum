#include "../../../includes/Model/Commands/ThrowNObjectsCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ThrowNObjectsCommand::ThrowNObjectsCommand() : Command() {}

bool ThrowNObjectsCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    return true;
}

ThrowNObjectsCommand::~ThrowNObjectsCommand() {}

//-----------------------------------------------------------------------------
