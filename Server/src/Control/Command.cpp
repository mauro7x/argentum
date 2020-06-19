#include "../../includes/Control/Command.h"

// Command::Command(InstanceId caller): caller(caller) {}
// Command::~Command() {}

// CommandProxy::CommandProxy(InstanceId caller, char cmd): 
//     Command(caller),
//     cmd(cmd) {}
// CommandProxy::~CommandProxy() {}

void CommandProxy::operator()(Game& game) {
    if (cmd == 'w') {
        game.startMovingUp(this->caller);
        return;
    }

    if (cmd == 'a') {
        game.startMovingLeft(this->caller);
        return;
    }

    if (cmd == 's') {
        game.startMovingDown(this->caller);
        return;
    }

    if (cmd == 'd') {
        game.startMovingRight(this->caller);
        return;
    }

    if (cmd == 'q') {
        game.stopMoving(this->caller);
        return;
    }
}   
