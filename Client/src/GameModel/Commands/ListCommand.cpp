#include "../../../includes/GameModel/Commands/ListCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ListCommand::ListCommand(uint32_t x, uint32_t y) : Command(), x(x), y(y) {}

bool ListCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)LIST_CMD)) {
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

ListCommand::~ListCommand() {}

//-----------------------------------------------------------------------------
