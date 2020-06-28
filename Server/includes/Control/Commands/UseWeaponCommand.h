#ifndef __USE_WEAPON_COMMAND_H__
#define __USE_WEAPON_COMMAND_H__

#include "Command.h"

class UseWeaponCommand : public Command {
   private:
    InstanceId caller;

    const InstanceId target;

   public:
    UseWeaponCommand(InstanceId caller, const InstanceId target);
    ~UseWeaponCommand();

    void exec(Game& game) override;
};

#endif  // __USE_WEAPON_COMMAND_H__
