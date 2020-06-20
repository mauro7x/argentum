#include "../../includes/Control/Command.h"

//-----------------------------------------------------------------------------
// API Pública

CommandMovement::CommandMovement(InstanceId caller, char cmd,
                                 BlockingQueue<Notification*>& notifications)
    : caller(caller), cmd(cmd), notifications(notifications) {}

void CommandMovement::exec(Game& game) {
    NotificationReply* reply = NULL;
    switch (cmd) {
        case START_MOVING_UP_CMD: {
            reply = game.startMovingUp(caller);
            notifications.push(reply);
            break;
        }
        case START_MOVING_DOWN_CMD: {
            reply = game.startMovingDown(caller);
            notifications.push(reply);
            break;
        }
        case START_MOVING_LEFT_CMD: {
            reply = game.startMovingLeft(caller);
            notifications.push(reply);
            break;
        }
        case START_MOVING_RIGHT_CMD: {
            reply = game.startMovingRight(caller);
            notifications.push(reply);
            break;
        }
        case STOP_MOVING_CMD: {
            reply = game.stopMoving(caller);
            notifications.push(reply);
            break;
        }
        default:
            break;
    }
}

//-----------------------------------------------------------------------------
