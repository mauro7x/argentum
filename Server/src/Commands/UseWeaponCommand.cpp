#include "../../includes/Control/Commands/UseWeaponCommand.h"

UseWeaponCommand::UseWeaponCommand(InstanceId caller, const uint32_t x_coord,
                                   const uint32_t y_coord)
    : caller(caller), x_coord(x_coord), y_coord(y_coord) {}

UseWeaponCommand::~UseWeaponCommand() {}

void UseWeaponCommand::exec(Game& game) {
    game.useWeapon(caller, x_coord, y_coord);
}
