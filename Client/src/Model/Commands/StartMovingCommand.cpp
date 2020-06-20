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
    ssize_t sent = 0;

    sent = (socket << (char)COMMAND_OPCODE);
    if (!sent) {
        return false;
    }

    switch (dir) {
        case UP_DIR: {
            sent = (socket << (char)START_MOVING_UP_CMD);
            break;
        }

        case DOWN_DIR: {
            sent = (socket << (char)START_MOVING_DOWN_CMD);
            break;
        }

        case LEFT_DIR: {
            sent = (socket << (char)START_MOVING_LEFT_CMD);
            break;
        }

        case RIGHT_DIR: {
            sent = (socket << (char)START_MOVING_RIGHT_CMD);
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
