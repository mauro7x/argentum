#include "../../../includes/GameModel/Commands/DepositNObjectsCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

DepositNObjectsCommand::DepositNObjectsCommand(uint32_t x, uint32_t y,
                                               uint8_t inventory_slot,
                                               uint32_t amount)
    : Command(), x(x), y(y), inventory_slot(inventory_slot), amount(amount) {}

bool DepositNObjectsCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)DEPOSIT_N_OBJECTS_CMD)) {
        return false;
    }

    if (!(socket << (uint32_t)x)) {
        return false;
    }

    if (!(socket << (uint32_t)y)) {
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

DepositNObjectsCommand::~DepositNObjectsCommand() {}

//-----------------------------------------------------------------------------
