#include "../../includes/Control/ActiveClients.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ActiveClients::ActiveClients(Queue<Command*>& commands,
                             Queue<InstanceId*>& finished_connections)
    : commands(commands), finished_connections(finished_connections) {}

void ActiveClients::add(const InstanceId id, SocketWrapper& peer) {
    // Lanzar alguna excepción si el id ya existe en el container.
    fprintf(stderr, "Agregando un cliente a la lista de clientes activos.\n");

    content.emplace(
        id, new ClientConnection(id, peer, finished_connections, commands));
    content.at(id)->start();
}

void ActiveClients::remove(const InstanceId id) {
    // Lanzar alguna excepción si el cliente no terminó.

    content.at(id)->join(); /* join NO bloqueante, pues la conexión terminó */
    delete content.at(id);
    content.erase(id);
}

void ActiveClients::stop() {
    for (auto it = content.begin(); it != content.end(); it++) {
        it->second->stop();
        it->second->join();
        delete it->second;
    }

    content.clear();
}

ActiveClients::~ActiveClients() {
    // hacer free de todo el contenido
    for (auto it = content.begin(); it != content.end(); it++) {
        delete it->second;
    }

    content.clear();
}

//-----------------------------------------------------------------------------
