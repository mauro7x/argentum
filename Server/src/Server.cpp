#include "../includes/Server.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Server::Server(const std::string& port, unsigned int max_clients_queued) {}

void Server::run() {
    std::cout << "Comienza la ejecución del servidor." << std::endl;

    // Ejecución del cliente

    std::cout << "Termina la ejecución del servidor." << std::endl;
}

Server::~Server() {}

//-----------------------------------------------------------------------------
