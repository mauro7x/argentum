#include "../../../includes/GameModel/Commands/SelfResurrectCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

SelfResurrectCommand::SelfResurrectCommand() : Command() {}

bool SelfResurrectCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)SELF_RESURRECT_CMD)) {
        return false;
    }

    return true;
}

SelfResurrectCommand::~SelfResurrectCommand() {}

//-----------------------------------------------------------------------------
