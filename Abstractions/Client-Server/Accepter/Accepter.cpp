#include "Accepter.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

void Accepter::_joinAndFreeLoggedConnections() {
    active_logins.remove_if();
}

Accepter::Accepter(const std::string& port, unsigned int max_clients_queued)
    : socket(port, max_clients_queued),
      keep_listening(true),
      is_running(false) {}

void Accepter::run() {
    is_running = true;

    try {
        while (keep_listening) {
            // aceptar clientes
            SocketWrapper new_sock = socket.accept();
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
