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

    // Iniciamos la base de datos
    database.init();

    // Iniciamos el motor
    engine.start();

    // Iniciamos el accepter
    accepter.start();

    /* Loop de comandos */
    std::string input;
    do {
        std::getline(std::cin, input);
    } while (input != "q");

    // Salimos ordenadamente
    accepter.stop();
    accepter.join();

    engine.stop();
    engine.join();

    fprintf(stderr, "DEBUG: Termina la ejecución del servidor.\n");
}

Server::~Server() {}

//-----------------------------------------------------------------------------
