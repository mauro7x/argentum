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

    sent = (socket << (char)COMMAND_OPCODE);
    if (!sent) {
        return false;
    }

    sent = (socket << (char)MEDITATE_CMD);
    if (!sent) {
        return false;
    }

    return true;
}

MeditateCommand::~MeditateCommand() {}

//-----------------------------------------------------------------------------
