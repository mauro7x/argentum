#ifndef __SELF_RESURRECT_COMMAND_H__
#define __SELF_RESURRECT_COMMAND_H__

#include "Command.h"

class SelfResurrectCommand : public Command {
   private:
    InstanceId caller;

   public:
    SelfResurrectCommand(InstanceId caller);
    ~SelfResurrectCommand();

    void exec(Game& game) override;
};

#endif // __RESURRECT_COMMAND_H__

