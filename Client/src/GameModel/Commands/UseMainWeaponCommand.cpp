#include "../../../includes/GameModel/Commands/UseMainWeaponCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UseMainWeaponCommand::UseMainWeaponCommand(const InstanceId& target)
    : Command(), target(target) {}

bool UseMainWeaponCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)USE_MAIN_WEAPON_CMD)) {
        return false;
    }

    if (!(socket << (uint32_t)target)) {
        return false;
    }

    return true;
}

UseMainWeaponCommand::~UseMainWeaponCommand() {}

//-----------------------------------------------------------------------------
