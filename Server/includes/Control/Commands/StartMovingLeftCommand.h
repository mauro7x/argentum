#ifndef __START_MOVING_LEFT_COMMAND_H__
#define __START_MOVING_LEFT_COMMAND_H__

#include "Command.h"

class StartMovingLeftCommand : public Command {
   private:
    InstanceId caller;

   public:
    StartMovingLeftCommand(InstanceId caller);
    ~StartMovingLeftCommand();

    void exec(Game& game) override;
};

#endif  // __START_MOVING_LEFT_COMMAND_H__
