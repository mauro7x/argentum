#ifndef __USE_WEAPON_COMMAND_H__
#define __USE_WEAPON_COMMAND_H__

#include "Command.h"

class UseWeaponCommand : public Command {
   private:
    InstanceId caller;

    const uint32_t x_coord;
    const uint32_t y_coord;

   public:
    UseWeaponCommand(InstanceId caller, const uint32_t x_coord,
                     const uint32_t y_coord);
    ~UseWeaponCommand();

    void exec(Game& game) override;
};

#endif // __USE_WEAPON_COMMAND_H__
