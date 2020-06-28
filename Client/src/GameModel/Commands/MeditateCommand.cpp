#include "../../../includes/GameModel/Commands/MeditateCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

MeditateCommand::MeditateCommand() : Command() {}

bool MeditateCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)MEDITATE_CMD)) {
        return false;
    }

    return true;
}

MeditateCommand::~MeditateCommand() {}

//-----------------------------------------------------------------------------
