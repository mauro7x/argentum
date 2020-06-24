#include "../../../includes/Model/Commands/MeditateCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

MeditateCommand::MeditateCommand() : Command() {}

bool MeditateCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    ssize_t sent = 0;

    sent = (socket << (uint8_t)COMMAND_OPCODE);
    if (!sent) {
        return false;
    }

    sent = (socket << (uint8_t)MEDITATE_CMD);
    if (!sent) {
        return false;
    }

    return true;
}

MeditateCommand::~MeditateCommand() {}

//-----------------------------------------------------------------------------
