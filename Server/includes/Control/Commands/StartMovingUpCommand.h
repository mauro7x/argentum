#ifndef __START_MOVING_UP_COMMAND_H__
#define __START_MOVING_UP_COMMAND_H__

#include "Command.h"

// Empezar a moverse abajo.
class StartMovingUpCommand : public Command {
   public:
    StartMovingUpCommand(const InstanceId caller);
    ~StartMovingUpCommand();

    void exec(Game& game) override;
};

#endif  // __START_MOVING_UP_COMMAND_H__
