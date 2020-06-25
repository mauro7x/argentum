#include "../../../includes/Model/Commands/HealCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

HealCommand::HealCommand() : Command() {}

bool HealCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    return true;
}

HealCommand::~HealCommand() {}

//-----------------------------------------------------------------------------
