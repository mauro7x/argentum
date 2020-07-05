#ifndef __START_MOVING_DOWN_COMMAND_H__
#define __START_MOVING_DOWN_COMMAND_H__

#include "Command.h"

// Empezar a moverse abajo.
class StartMovingDownCommand : public Command {
   public:
    StartMovingDownCommand(const InstanceId caller);
    ~StartMovingDownCommand();

    void exec(Game& game) override;
};

#endif  // __START_MOVING_DOWN_COMMAND_H__
