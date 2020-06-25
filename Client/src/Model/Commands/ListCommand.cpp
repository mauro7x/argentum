#include "../../../includes/Model/Commands/ListCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ListCommand::ListCommand() : Command() {}

bool ListCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    return true;
}

ListCommand::~ListCommand() {}

//-----------------------------------------------------------------------------
