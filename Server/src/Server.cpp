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
    auto next = steady_clock::now();
    int time_delta = 0;
    while (!ended) {
        time_delta = 1;
        next += Framerate{ 1 };
        while (next < steady_clock::now()) {
            time_delta++;
            next += Framerate{ 1 };
        }
        std::this_thread::sleep_until(next);

        processPlayerCommands();
        processNpcTurns();

        // Habria que implementar una situacion donde se guarda

    }

    // Guardar estado una vez mas 
}

void Server::processPlayerTurns(int time_delta) {
}

void Server::processPlayerCommands() {
}

void Server::processNpcTurns(int time_delta) {
}

Server::~Server() {}

//-----------------------------------------------------------------------------
