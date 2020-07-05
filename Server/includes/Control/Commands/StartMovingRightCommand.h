#ifndef __START_MOVING_RIGHT_COMMAND_H__
#define __START_MOVING_RIGHT_COMMAND_H__

#include "Command.h"

// Empezar a moverse a la derecha.
class StartMovingRightCommand : public Command {
   public:
    StartMovingRightCommand(const InstanceId caller);
    ~StartMovingRightCommand();

    void exec(Game& game) override;
};

#endif  // __START_MOVING_RIGHT_COMMAND_H__
