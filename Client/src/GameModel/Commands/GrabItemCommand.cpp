#include "../../../includes/GameModel/Commands/GrabItemCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

GrabItemCommand::GrabItemCommand() : Command() {}

bool GrabItemCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)GRAB_OBJECT_CMD)) {
        return false;
    }

    return true;
}

GrabItemCommand::~GrabItemCommand() {}

//-----------------------------------------------------------------------------
