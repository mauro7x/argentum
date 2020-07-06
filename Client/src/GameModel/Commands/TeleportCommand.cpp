#include "../../../includes/GameModel/Commands/TeleportCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

TeleportCommand::TeleportCommand(uint32_t x, uint32_t y, uint32_t map_id)
    : Command(), x(x), y(y), map_id(map_id) {}

bool TeleportCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)TELEPORT_CMD)) {
        return false;
    }

    if (!(socket << (uint32_t)x)) {
        return false;
    }

    if (!(socket << (uint32_t)y)) {
        return false;
    }

    if (!(socket << (uint32_t)map_id)) {
        return false;
    }

    return true;
}

TeleportCommand::~TeleportCommand() {}

//-----------------------------------------------------------------------------
