#include "../../../includes/Model/Commands/UnequipObjectCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

UnequipObjectCommand::UnequipObjectCommand(int8_t equipment_slot)
    : Command(), equipment_slot(equipment_slot) {}

bool UnequipObjectCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    if (!(socket << (uint8_t)UNEQUIP_OBJECT_CMD)) {
        return false;
    }

    if (!(socket << (uint8_t)equipment_slot)) {
        return false;
    }

    return true;
}

UnequipObjectCommand::~UnequipObjectCommand() {}

//-----------------------------------------------------------------------------
