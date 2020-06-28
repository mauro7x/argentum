#include "../../../includes/GameModel/Commands/HelpNpcCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

HelpNpcCommand::HelpNpcCommand(uint32_t x, uint32_t y)
    : Command(), x(x), y(y) {}

bool HelpNpcCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)HELP_NPC_CMD)) {
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

HelpNpcCommand::~HelpNpcCommand() {}

//-----------------------------------------------------------------------------
