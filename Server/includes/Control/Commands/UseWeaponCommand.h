#ifndef __USE_WEAPON_COMMAND_H__
#define __USE_WEAPON_COMMAND_H__

#include "Command.h"

// Usar el arma sobre el target.
class UseWeaponCommand : public Command {
   private:
    const InstanceId target;

   public:
    UseWeaponCommand(const InstanceId caller, const InstanceId target);
    ~UseWeaponCommand();

    void exec(Game& game) override;
};

#endif  // __USE_WEAPON_COMMAND_H__
