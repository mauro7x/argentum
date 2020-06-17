#include "../../includes/Model/CommandDispatcher.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

CommandDispatcher::CommandDispatcher(const SocketWrapper& socket,
                                     BlockingQueue<Command*>& commands,
                                     std::atomic_bool& exit)
    : socket(socket), commands(commands), exit(exit) {}

void CommandDispatcher::run() {
    try {
        Command* command = NULL;
        while ((command = commands.pop())) {
            if (!command->send(socket)) {
                // Se cerró el socket y hay que terminar
                break;
            }
            delete command;
        }
    } catch (const std::exception& e) {
        fprintf(stderr, "CommandDispatcher // %s\n", e.what());
    } catch (...) {
        fprintf(stderr, "CommandDispatcher // Unknown error.\n");
    }

    // Avisamos que se cerró el socket y que hay que terminar
    exit = true;
}

CommandDispatcher::~CommandDispatcher() {}

//-----------------------------------------------------------------------------
