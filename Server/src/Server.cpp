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

Server::Server(const std::string& port, unsigned int max_clients_queued) {

}

void Server::run() {
    using namespace std::chrono;

    // Esto define el framerate
    using Framerate = duration<steady_clock::rep, std::ratio<1, 30>>;
    auto next = steady_clock::now() + Framerate{ 1 };

    while (1) {
        processPlayerCommands();
        processNpcTurns();

        std::this_thread::sleep_until(next);
        next += Framerate{ 1 };
    }
}

void Server::processPlayerCommands() {
    players.executeCommand();
}

void Server::processNpcTurns() {
}

void Server::passTurn() {
}

Server::~Server() {}

//-----------------------------------------------------------------------------
