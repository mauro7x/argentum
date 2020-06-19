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
            reply = std::move(game.startMovingUp(caller));
            reply.send(peer);
            break;
        }
        case START_MOVING_DOWN_CMD: {
            reply = std::move(game.startMovingDown(caller));
            reply.send(peer);
            break;
        }
        case START_MOVING_LEFT_CMD: {
            reply = std::move(game.startMovingLeft(caller));
            reply.send(peer);
            break;
        }
        case START_MOVING_RIGHT_CMD: {
            reply = std::move(game.startMovingRight(caller));
            reply.send(peer);
            break;
        }
        case STOP_MOVING_CMD: {
            reply = std::move(game.stopMoving(caller));
            reply.send(peer);
            break;
        }
        default:
            break;
    }
}

//-----------------------------------------------------------------------------
