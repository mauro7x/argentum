#include "../../../includes/Model/Commands/StartMovingCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

StartMovingCommand::StartMovingCommand(MovingDirection dir)
    : Command(), dir(dir) {}

bool StartMovingCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    ssize_t sent = 0;

    switch (dir) {
        case UP_DIR: {
            sent = (socket << (uint8_t)START_MOVING_UP_CMD);
            break;
        }

        case DOWN_DIR: {
            sent = (socket << (uint8_t)START_MOVING_DOWN_CMD);
            break;
        }

        case LEFT_DIR: {
            sent = (socket << (uint8_t)START_MOVING_LEFT_CMD);
            break;
        }

        case RIGHT_DIR: {
            sent = (socket << (uint8_t)START_MOVING_RIGHT_CMD);
            break;
        }
    }

    if (!sent) {
        return false;
    }

    return true;
}

StartMovingCommand::~StartMovingCommand() {}

//-----------------------------------------------------------------------------
