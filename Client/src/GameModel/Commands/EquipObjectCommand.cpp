#include "../../../includes/GameModel/Commands/EquipObjectCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

EquipObjectCommand::EquipObjectCommand(int8_t inventory_slot)
    : Command(), inventory_slot(inventory_slot) {}

bool EquipObjectCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)EQUIP_OBJECT_CMD)) {
        return false;
    }

    if (!(socket << (uint8_t)inventory_slot)) {
        return false;
    }

    return true;
}

EquipObjectCommand::~EquipObjectCommand() {}

//-----------------------------------------------------------------------------
