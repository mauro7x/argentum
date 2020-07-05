#include "../../includes/Control/Server.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Server::Server(const std::string& port, const int max_clients_queued)
    : database(),
      new_connections(),
      accepter(port, max_clients_queued, database, new_connections),
      engine(database, new_connections) {}

void Server::run() {
    fprintf(stderr, "DEBUG: Comienza la ejecución del servidor.\n");

    // Iniciamos la ejecución del motor
    engine.start();

    // Iniciamos la ejecución del accepter
    accepter.start();

    /** LOOP DE COMANDOS
     *
     * 'q' = cierra el servidor.
     */
    std::string input;
    do {
        std::getline(std::cin, input);
    } while (input != "q");

    // Salimos ordenadamente
    fprintf(stderr, "DEBUG: Stoppeando al accepter.\n");
    accepter.stop();
    fprintf(stderr, "DEBUG: Joineando al accepter.\n");
    accepter.join();
    fprintf(stderr, "DEBUG: Accepter joineado.\n");

    fprintf(stderr, "DEBUG: Stoppeando al accepter.\n");
    engine.stop();
    fprintf(stderr, "DEBUG: Joineando al engine.\n");
    engine.join();
    fprintf(stderr, "DEBUG: Engine joineado.\n");

    fprintf(stderr, "DEBUG: Termina la ejecución del servidor.\n");
}

Server::~Server() {}

//-----------------------------------------------------------------------------
