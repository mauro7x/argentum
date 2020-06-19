#include "../../includes/Control/Command.h"

//-----------------------------------------------------------------------------
// API Pública

CommandMovement::CommandMovement(InstanceId caller, char cmd,
                                 SocketWrapper& peer)
    : caller(caller), cmd(cmd), peer(peer) {}

void CommandMovement::operator()(Game& game) {
    NotificationReply reply;
    switch (cmd) {
        case START_MOVING_UP_CMD: {
            reply = game.startMovingUp(caller);
            reply.send(peer);
            break;
        }
        case START_MOVING_DOWN_CMD: {
            reply = game.startMovingDown(caller);
            reply.send(peer);
            break;
        }
        case START_MOVING_LEFT_CMD: {
            reply = game.startMovingLeft(caller);
            reply.send(peer);
            break;
        }
        case START_MOVING_RIGHT_CMD: {
            reply = game.startMovingRight(caller);
            reply.send(peer);
            break;
        }
        case STOP_MOVING_CMD: {
            reply = game.stopMoving(caller);
            reply.send(peer);
            break;
        }
        default:
            break;
    }
}

//-----------------------------------------------------------------------------
