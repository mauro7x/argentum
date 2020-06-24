#ifndef __START_MOVING_UP_COMMAND_H__
#define __START_MOVING_UP_COMMAND_H__

#include "Command.h"

class StartMovingUpCommand : public Command {
   private:
    InstanceId caller;

   public:
    StartMovingUpCommand(InstanceId caller);
    ~StartMovingUpCommand();

    void exec(Game& game) override;
};

#endif  // __START_MOVING_UP_COMMAND_H__
