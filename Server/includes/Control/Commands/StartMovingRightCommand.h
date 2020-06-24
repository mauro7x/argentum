#ifndef __START_MOVING_RIGHT_COMMAND_H__
#define __START_MOVING_RIGHT_COMMAND_H__

#include "Command.h"

class StartMovingRightCommand : public Command {
   private:
    InstanceId caller;

   public:
    StartMovingRightCommand(InstanceId caller);
    ~StartMovingRightCommand();

    void exec(Game& game) override;
};

#endif  // __START_MOVING_RIGHT_COMMAND_H__
