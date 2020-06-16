#include "../includes/Server.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "../includes/ClientList.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Server::Server(const std::string& port, unsigned int max_clients_queued)
    : accepter(port, max_clients_queued, &players, &c_q) {}

void Server::run() {
    accepter.start();
    using namespace std::chrono;

    // Esto define el framerate
    using Framerate = duration<steady_clock::rep, std::ratio<1, 30>>;
    auto next = steady_clock::now() + Framerate{0};
    int time_delta = 0;
    while (!ended) {
        time_delta = 1;
        next += Framerate{1};
        while (next < steady_clock::now()) {
            time_delta++;
            next += Framerate{1};
        }
        std::this_thread::sleep_until(next);

        processPlayerTurns(time_delta);
        processPlayerCommands();
        processNpcTurns(time_delta);

        // Habria que implementar una situacion donde se guarda
    }

    accepter.join();
    // Guardar estado una vez mas
}

void Server::processPlayerTurns(int time_delta) {}

void Server::processPlayerCommands() {}

void Server::processNpcTurns(int time_delta) {}

Server::~Server() {}

//-----------------------------------------------------------------------------
