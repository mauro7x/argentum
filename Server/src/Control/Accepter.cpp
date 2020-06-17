#include "../../includes/Control/Accepter.h"

//-----------------------------------------------------------------------------
// Métodos privados

void Accepter::_acceptClient() {
    SocketWrapper peer = std::move(socket.accept());
    ClientLogin* new_client = new ClientLogin(peer, database, new_connections);
    new_client->start();
    client_logins.push_back(new_client);
}

void Accepter::_joinFinishedLogins() {
    std::vector<ClientLogin*> tmp;

    for (auto it = client_logins.begin(); it != client_logins.end(); it++) {
        if (!(*it)->isRunning()) {
            (*it)->join(); /* este join NO es bloqueante */
            delete *it;
        } else {
            tmp.push_back(*it);
        }
    }

    client_logins.swap(tmp);
}

void Accepter::_joinLogins() {
    for (auto it = client_logins.begin(); it != client_logins.end(); it++) {
        (*it)->stop();
        (*it)->join(); /* este join ES bloqueante */
        delete *it;
    }

    client_logins.clear();
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

    // En caso de salir con error, ver si hace falta cortar el server en gral.
}

void Accepter::stop() {
    keep_accepting = false;
    socket.shutdown();
    socket.close();
}

Accepter::~Accepter() {
    /* just in case */
    _joinLogins();
}

//-----------------------------------------------------------------------------
