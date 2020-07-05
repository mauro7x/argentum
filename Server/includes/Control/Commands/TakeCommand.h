#ifndef __TAKE_COMMAND_H__
#define __TAKE_COMMAND_H__

#include "Command.h"

// /tomar. Toma el objeto en su tile.
class TakeCommand : public Command {
   public:
    TakeCommand(const InstanceId caller);
    ~TakeCommand();

    void exec(Game& game) override;
};

#endif  // __TAKE_COMMAND_H__
