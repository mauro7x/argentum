#include "../../../includes/GameModel/Commands/ThrowNObjectsCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ThrowNObjectsCommand::ThrowNObjectsCommand(uint8_t inventory_slot,
                                           uint32_t amount)
    : Command(), inventory_slot(inventory_slot), amount(amount) {}

bool ThrowNObjectsCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)THROW_N_OBJECTS_CMD)) {
        return false;
    }

    if (!(socket << (uint8_t)inventory_slot)) {
        return false;
    }

    if (!(socket << (uint32_t)amount)) {
        return false;
    }

    return true;
}

ThrowNObjectsCommand::~ThrowNObjectsCommand() {}

//-----------------------------------------------------------------------------
