#include "../../../includes/Control/Commands/UseWeaponCommand.h"

UseWeaponCommand::UseWeaponCommand(const InstanceId caller,
                                   const InstanceId target)
    : Command(caller), target(target) {}

UseWeaponCommand::~UseWeaponCommand() {}

void UseWeaponCommand::exec(Game& game) {
    game.useWeapon(caller, target);
}
