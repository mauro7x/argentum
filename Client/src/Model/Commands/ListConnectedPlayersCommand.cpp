#include "../../../includes/Model/Commands/ListConnectedPlayersCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ListConnectedPlayersCommand::ListConnectedPlayersCommand() : Command() {}

bool ListConnectedPlayersCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)LIST_CONNECTED_PLAYERS_CMD)) {
        return false;
    }

    return true;
}

ListConnectedPlayersCommand::~ListConnectedPlayersCommand() {}

//-----------------------------------------------------------------------------
