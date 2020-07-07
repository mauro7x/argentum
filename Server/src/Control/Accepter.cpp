#include "../../includes/Control/Accepter.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Accepter::_acceptClient() {
    SocketWrapper peer = std::move(socket.accept());
    client_logins.emplace_back(peer, database, new_connections);
    client_logins.back().start();
}

void Accepter::_joinFinishedLogins() {
    for (auto it = client_logins.begin(); it != client_logins.end();) {
        if (it->isRunning()) {
            it->join(); /* este join NO es bloqueante */
            it = client_logins.erase(it);
        } else {
            it++;
        }
    }
}

void Accepter::_joinLogins() {
    for (auto it = client_logins.begin(); it != client_logins.end();) {
        it->stop();
        it->join();
        it = client_logins.erase(it);
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Accepter::Accepter(const std::string& port, const int max_clients_queued,
                   Database& database,
                   NonBlockingQueue<NewConnection*>& new_connections)
    : socket(port, max_clients_queued),
      database(database),
      new_connections(new_connections),
      keep_accepting(true) {}

void Accepter::run() {
    // Hilo principal de ejecución del accepter

    try {
        while (keep_accepting) {
            _acceptClient();
            _joinFinishedLogins();
        }

    } catch (const ClosedSocketException& e) {
        // Orden de cerrar el accepter del server
        _joinLogins();

    } catch (const std::exception& e) {
        // Error grave
        _joinLogins();

        fprintf(stderr, "%s\n", e.what());
    } catch (...) {
        // Error desconocido
        _joinLogins();

        fprintf(stderr, "Unknown error.\n");
    }

    // Just in case
    _joinLogins();
}

void Accepter::stop() {
    keep_accepting = false;
    try {
        socket.shutdown();
    } catch (const Exception& e) {
        fprintf(stderr,
                "Warning: error while shutting-down accepter. Aborting.\n");
    }
    socket.close();
}

Accepter::~Accepter() {
    /* just in case */
    _joinLogins();
}

//-----------------------------------------------------------------------------
