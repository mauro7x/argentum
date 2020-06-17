#include "../../includes/Model/CommandDispatcher.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

CommandDispatcher::CommandDispatcher(const SocketWrapper& socket,
                                     BlockingQueue<Command*>& commands)
    : socket(socket), commands(commands) {}

void CommandDispatcher::run() {
    try {
        Command* command = NULL;
        while ((command = commands.pop())) {
            if (!command->send(socket)) {
                // no se pudo enviar porque el socket se cerró
                // commands.close()?
            }
            delete command;
        }
    } catch (const std::exception& e) {
        fprintf(stderr, "CommandDispatcher // %s\n", e.what());
    } catch (...) {
        fprintf(stderr, "CommandDispatcher // Unknown error.\n");
    }
}

CommandDispatcher::~CommandDispatcher() {}

//-----------------------------------------------------------------------------
