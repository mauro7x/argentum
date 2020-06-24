#ifndef __START_MOVING_DOWN_COMMAND_H__
#define __START_MOVING_DOWN_COMMAND_H__

#include "Command.h"

class StartMovingDownCommand : public Command {
   private:
    InstanceId caller;

   public:
    StartMovingDownCommand(InstanceId caller);
    ~StartMovingDownCommand();

    void exec(Game& game) override;
};

#endif  // __START_MOVING_DOWN_COMMAND_H__
