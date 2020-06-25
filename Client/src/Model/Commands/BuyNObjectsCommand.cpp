#include "../../../includes/Model/Commands/BuyNObjectsCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

BuyNObjectsCommand::BuyNObjectsCommand() : Command() {}

bool BuyNObjectsCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    return true;
}

BuyNObjectsCommand::~BuyNObjectsCommand() {}

//-----------------------------------------------------------------------------
