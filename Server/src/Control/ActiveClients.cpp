#include "../../includes/Control/ActiveClients.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ActiveClients::ActiveClients(
    NonBlockingQueue<Command*>& commands,
    NonBlockingQueue<InstanceId*>& finished_connections)
    : commands(commands), finished_connections(finished_connections) {}

void ActiveClients::add(const InstanceId id, const Id map,
                        SocketWrapper& peer) {
    if (content.count(id) > 0) {
        throw Exception("ActiveClients::add: repeated client id.");
    }

    content.emplace(id, new ClientConnection(id, map, peer,
                                             finished_connections, commands));
    content.at(id)->start();
}

void ActiveClients::remove(const InstanceId id) {
    if (content.count(id) == 0) {
        throw Exception("ActiveClients::remove: invalid client id.");
    }  // considerar tambien el caso en que su conexión no haya terminado

    content.at(id)->join(); /* join NO bloqueante, pues la conexión terminó */
    delete content.at(id);
    content.erase(id);
}

void ActiveClients::notify(const InstanceId& id, Notification* notification) {
    if (content.count(id) == 0) {
        throw Exception("ActiveClients::notify: invalid client id.");
    }

    content.at(id)->push(notification);
}

void ActiveClients::sendDifferentialBroadcastToAll(Notification* broadcast) {
    std::unordered_map<InstanceId, ClientConnection*>::iterator it =
        this->content.begin();

    Notification* broadcast_copy;

    while (it != this->content.end()) {
        broadcast_copy =
            new NotificationBroadcast(*((NotificationBroadcast*)broadcast));
        it->second->push(broadcast_copy);
        ++it;
    }

    delete broadcast;
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
