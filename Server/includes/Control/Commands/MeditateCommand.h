#ifndef __MEDITATE_COMMAND_H__
#define __MEDITATE_COMMAND_H__

#include "Command.h"

// /meditar. El jugador recupera su maná velozmente.
class MeditateCommand : public Command {
   public:
    MeditateCommand(const InstanceId caller);
    ~MeditateCommand();

    void exec(Game& game) override;
};

#endif  // __MEDITATE_COMMAND_H__
