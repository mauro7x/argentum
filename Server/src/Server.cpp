#include "../includes/Server.h"
#include "../includes/ClientList.h"
#include <iostream>
#include <chrono>
#include <thread>

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Server::Server(const std::string& port, unsigned int max_clients_queued):
    accepter(port, max_clients_queued, players){
}

void Server::run() {
    accepter.start();
    using namespace std::chrono;

    // Esto define el framerate
    using Framerate = duration<steady_clock::rep, std::ratio<1, 30>>;
    auto next = steady_clock::now() + Framerate{ 1 };

    while (!ended) {

        processPlayerCommands();
        processNpcTurns();

        // Habria que implementar una situacion donde se guarda

        std::this_thread::sleep_until(next);
        next += Framerate{ 1 };
    }

    // Guardar estado una vez mas 
}

void Server::processPlayerCommands() {
}

void Server::processNpcTurns() {
}

void Server::passTurn() {
}

Server::~Server() {}

//-----------------------------------------------------------------------------
