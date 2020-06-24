#ifndef __MEDITATE_COMMAND_H__
#define __MEDITATE_COMMAND_H__

#include "Command.h"

class MeditateCommand : public Command {
   private:
    InstanceId caller;

   public:
    MeditateCommand(InstanceId caller);
    ~MeditateCommand();

    void exec(Game& game) override;
};

#endif // __MEDITATE_COMMAND_H__
