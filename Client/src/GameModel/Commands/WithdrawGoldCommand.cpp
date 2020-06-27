#include "../../../includes/GameModel/Commands/WithdrawGoldCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

WithdrawGoldCommand::WithdrawGoldCommand(uint32_t x, uint32_t y,
                                         uint32_t amount)
    : Command(), x(x), y(y), amount(amount) {}

bool WithdrawGoldCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)WITHDRAW_GOLD_CMD)) {
        return false;
    }

    if (!(socket << (uint32_t)x)) {
        return false;
    }

    if (!(socket << (uint32_t)y)) {
        return false;
    }

    if (!(socket << (uint32_t)amount)) {
        return false;
    }

    return true;
}

WithdrawGoldCommand::~WithdrawGoldCommand() {}

//-----------------------------------------------------------------------------
