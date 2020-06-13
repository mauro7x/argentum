#include "Accepter.h"
#include <iostream>

//-----------------------------------------------------------------------------
// Métodos privados

void Accepter::_joinAndFreeLoggedConnections() {
    /*for (auto i = active_logins.begin(); i != active_logins.end();) {
        if (i->isRunning())
            i = active_logins.erase(i);
        else
            ++i;
    }*/
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Accepter::Accepter(const std::string& port, unsigned int max_clients_queued, 
                   ClientList* cli, CommandQueue* command_queue) :
    socket(port, max_clients_queued),
    keep_listening(true),
    is_running(false) {
    clients = cli;
    c_q = command_queue;
}

void Accepter::run() {
    is_running = true;

    try {
        while (keep_listening) {
            // aceptar clientes
            SocketWrapper new_sock = socket.accept();
            ClientLogin new_login(clients, &log_val, std::move(new_sock));
            active_logins.push_back(std::move(new_login));
            active_logins.back().start();
            // implementar garbage collector
        }
    } catch (const ClosedSocketException& e) {
        // Manejar la órden de dejar de aceptar clientes
    } catch (const Exception& e) {
        // Manejar una excepción general
    } catch (...) {
        // Manejar un error desconocido
    }

    is_running = false;
}

bool Accepter::isRunning() {
    return is_running;
}

void Accepter::close() {
    keep_listening = false;
    socket.shutdown();
    socket.close();
}

Accepter::~Accepter() {}

//-----------------------------------------------------------------------------
