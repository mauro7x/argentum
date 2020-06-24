#ifndef __RESURRECT_COMMAND_H__
#define __RESURRECT_COMMAND_H__

#include "Command.h"

class ResurrectCommand : public Command {
   private:
    InstanceId caller;

   public:
    ResurrectCommand(InstanceId caller);
    ~ResurrectCommand();

    void exec(Game& game) override;
};

#endif // __RESURRECT_COMMAND_H__

