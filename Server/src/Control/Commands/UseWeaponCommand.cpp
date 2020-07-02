#include "../../../includes/Control/Commands/UseWeaponCommand.h"

UseWeaponCommand::UseWeaponCommand(InstanceId caller, const InstanceId target)
    : caller(caller), target(target) {}

UseWeaponCommand::~UseWeaponCommand() {}

void UseWeaponCommand::exec(Game& game) {
    game.useWeapon(caller, target);
}
