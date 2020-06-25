#include "../../../includes/Model/Commands/SellNObjectsCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

SellNObjectsCommand::SellNObjectsCommand() : Command() {}

bool SellNObjectsCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    return true;
}

SellNObjectsCommand::~SellNObjectsCommand() {}

//-----------------------------------------------------------------------------
