#include "../../includes/Control/Command.h"

//-----------------------------------------------------------------------------
// API Pública

CommandMovement::CommandMovement(InstanceId caller, char cmd,
                                 BlockingQueue<Notification*>& notifications)
    : caller(caller), cmd(cmd), notifications(notifications) {}

void CommandMovement::operator()(Game& game) {
    NotificationReply reply;
    switch (cmd) {
        case START_MOVING_UP_CMD: {
            reply = std::move(game.startMovingUp(caller));
            notifications.push(&reply);
            break;
        }
        case START_MOVING_DOWN_CMD: {
            reply = std::move(game.startMovingDown(caller));
            notifications.push(&reply);
            break;
        }
        case START_MOVING_LEFT_CMD: {
            reply = std::move(game.startMovingLeft(caller));
            notifications.push(&reply);
            break;
        }
        case START_MOVING_RIGHT_CMD: {
            reply = std::move(game.startMovingRight(caller));
            notifications.push(&reply);
            break;
        }
        case STOP_MOVING_CMD: {
            reply = std::move(game.stopMoving(caller));
            notifications.push(&reply);
            break;
        }
        default:
            break;
    }
}

//-----------------------------------------------------------------------------
