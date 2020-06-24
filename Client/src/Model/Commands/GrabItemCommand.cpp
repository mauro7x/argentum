#include "../../../includes/Model/Commands/GrabItemCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

GrabItemCommand::GrabItemCommand() : Command() {}

bool GrabItemCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    ssize_t sent = 0;

    sent = (socket << (uint8_t)COMMAND_OPCODE);
    if (!sent) {
        return false;
    }

    sent = (socket << (uint8_t)GRAB_OBJECT_CMD);
    if (!sent) {
        return false;
    }

    return true;
}

GrabItemCommand::~GrabItemCommand() {}

//-----------------------------------------------------------------------------
