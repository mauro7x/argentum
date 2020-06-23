#include "../../includes/Control/Commands/Command.h"

//-----------------------------------------------------------------------------
// API Pública

CommandMovement::CommandMovement(InstanceId caller, char cmd)
    : caller(caller), cmd(cmd) {}

void CommandMovement::exec(Game& game) {
    switch (cmd) {
        case START_MOVING_UP_CMD: {
            game.startMovingUp(caller);
            break;
        }
        case START_MOVING_DOWN_CMD: {
            game.startMovingDown(caller);
            break;
        }
        case START_MOVING_LEFT_CMD: {
            game.startMovingLeft(caller);
            break;
        }
        case START_MOVING_RIGHT_CMD: {
            game.startMovingRight(caller);
            break;
        }
        case STOP_MOVING_CMD: {
            game.stopMoving(caller);
            break;
        }
        default:
            // lanzar alguna excepción
            break;
    }
}

//-----------------------------------------------------------------------------
