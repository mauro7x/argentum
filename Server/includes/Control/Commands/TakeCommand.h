#ifndef __TAKE_COMMAND_H__
#define __TAKE_COMMAND_H__

#include "Command.h"

class TakeCommand : public Command {
   private:
    InstanceId caller;

   public:
    TakeCommand(InstanceId caller);
    ~TakeCommand();

    void exec(Game& game) override;
};

#endif // __TAKE_COMMAND_H__

