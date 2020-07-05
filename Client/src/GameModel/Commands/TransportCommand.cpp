#include "../../../includes/GameModel/Commands/TransportCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

TransportCommand::TransportCommand(uint32_t x, uint32_t y)
    : Command(), x(x), y(y) {}

bool TransportCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)TRANSPORT_CMD)) {
        return false;
    }

    if (!(socket << (uint32_t)x)) {
        return false;
    }

    if (!(socket << (uint32_t)y)) {
        return false;
    }

    return true;
}

TransportCommand::~TransportCommand() {}

//-----------------------------------------------------------------------------
