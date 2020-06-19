#include "../../../includes/Model/Commands/StartMovementCommand.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

StartMovementCommand::StartMovementCommand(MovementDirection dir)
    : Command(), dir(dir) {}

bool StartMovementCommand::send(const SocketWrapper& socket) {
    // Enviamos el comando según el protocolo
    switch (dir) {
        case UP_DIR: {
            // implementar
            break;
        }

        case DOWN_DIR: {
            // implementar
            break;
        }

        case LEFT_DIR: {
            // implementar
            break;
        }

        case RIGHT_DIR: {
            // implementar
            break;
        }
    }
}

StartMovementCommand::~StartMovementCommand() {}

//-----------------------------------------------------------------------------
