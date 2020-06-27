#include "../../includes/GameModel/CommandDispatcher.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

CommandDispatcher::CommandDispatcher(const SocketWrapper& socket,
                                     BlockingQueue<Command*>& commands,
                                     GameView& game_view)
    : socket(socket), commands(commands), game_view(game_view) {}

void CommandDispatcher::run() {
    try {
        Command* command = NULL;
        bool socket_valid = true;
        while ((command = commands.pop())) {
            socket_valid = command->send(socket);
            delete command;

            if (!socket_valid) {
                // Se cerró el socket y hay que terminar
                break;
            }
        }
    } catch (const std::exception& e) {
        fprintf(stderr, "CommandDispatcher // %s\n", e.what());
    } catch (...) {
        fprintf(stderr, "CommandDispatcher // Unknown error.\n");
    }

    // Avisamos que se cerró el socket y que hay que terminar
    game_view.quit();
}

CommandDispatcher::~CommandDispatcher() {}

//-----------------------------------------------------------------------------
