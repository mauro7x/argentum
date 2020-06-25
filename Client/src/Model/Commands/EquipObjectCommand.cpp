#include "../../../includes/Model/Commands/EquipObjectCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

EquipObjectCommand::EquipObjectCommand() : Command() {}

bool EquipObjectCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    if (!(socket << (uint8_t)COMMAND_OPCODE)) {
        return false;
    }

    return true;
}

EquipObjectCommand::~EquipObjectCommand() {}

//-----------------------------------------------------------------------------
