#ifndef __STOP_MOVING_COMMAND_H__
#define __STOP_MOVING_COMMAND_H__

#include "Command.h"

class StopMovingCommand : public Command {
   public:
    StopMovingCommand(const InstanceId caller);
    ~StopMovingCommand();

    void exec(Game& game) override;
};

#endif  // __STOP_MOVING_COMMAND_H__
