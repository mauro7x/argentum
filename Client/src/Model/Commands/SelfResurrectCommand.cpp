#include "../../../includes/Model/Commands/SelfResurrectCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

SelfResurrectCommand::SelfResurrectCommand() : Command() {}

bool SelfResurrectCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    ssize_t sent = 0;

    sent = (socket << (uint8_t)COMMAND_OPCODE);
    if (!sent) {
        return false;
    }

    sent = (socket << (uint8_t)SELF_RESURRECT_CMD);
    if (!sent) {
        return false;
    }

    return true;
}

SelfResurrectCommand::~SelfResurrectCommand() {}

//-----------------------------------------------------------------------------