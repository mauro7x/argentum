#include "../../../includes/GameModel/Commands/BuyNObjectsCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

BuyNObjectsCommand::BuyNObjectsCommand(uint32_t x, uint32_t y, uint32_t item_id,
                                       uint32_t amount)
    : Command(), x(x), y(y), item_id(item_id), amount(amount) {}

bool BuyNObjectsCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)BUY_N_OBJECTS_CMD)) {
        return false;
    }

    if (!(socket << (uint32_t)x)) {
        return false;
    }

    if (!(socket << (uint32_t)y)) {
        return false;
    }

    if (!(socket << (uint32_t)item_id)) {
        return false;
    }

    if (!(socket << (uint32_t)amount)) {
        return false;
    }

    return true;
}

BuyNObjectsCommand::~BuyNObjectsCommand() {}

//-----------------------------------------------------------------------------
