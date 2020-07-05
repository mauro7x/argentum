#ifndef __SELF_RESURRECT_COMMAND_H__
#define __SELF_RESURRECT_COMMAND_H__

#include "Command.h"

// Resucita a un jugador muerto.
class SelfResurrectCommand : public Command {
   public:
    SelfResurrectCommand(const InstanceId caller);
    ~SelfResurrectCommand();

    void exec(Game& game) override;
};

#endif  // __RESURRECT_COMMAND_H__
