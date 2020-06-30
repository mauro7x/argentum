#include "../../includes/Control/ActiveClients.h"
//-----------------------------------------------------------------------------
#include "../../includes/Control/EntityBroadcast.h"
#include "../../includes/Control/ItemBroadcast.h"
#include "../../includes/Control/Message.h"
//-----------------------------------------------------------------------------

ActiveClients::ActiveClients(
    NonBlockingQueue<Command*>& commands,
    NonBlockingQueue<InstanceId*>& finished_connections)
    : commands(commands), finished_connections(finished_connections) {}

void ActiveClients::add(const InstanceId id, const Id map,
                        SocketWrapper& peer) {
    if (content.count(id) > 0) {
        throw Exception("ActiveClients::add: repeated client id.");
    }

    content.emplace(
        std::piecewise_construct, std::forward_as_tuple(id),
        std::forward_as_tuple(id, map, peer, finished_connections, commands));
    content.at(id).start();
}

void ActiveClients::remove(const InstanceId id) {
    if (content.count(id) == 0) {
        throw Exception("ActiveClients::remove: invalid client id.");
    }  // considerar tambien el caso en que su conexión no haya terminado

    content.at(id).join(); /* join NO bloqueante, pues la conexión terminó */
    content.erase(id);
}

void ActiveClients::notify(const InstanceId& id, Notification* notification) {
    if (content.count(id) == 0) {
        throw Exception("ActiveClients::notify: invalid client id.");
    }

    content.at(id).push(notification);
}

void ActiveClients::sendDifferentialBroadcastToAll(
    Notification* broadcast, const InstanceId updated_client,
    const bool send_to_updated_client) {
    auto it = content.begin();

    Notification* broadcast_copy;

    while (it != content.end()) {
        if (it->first == updated_client && (!send_to_updated_client)) {
            ++it;
            continue;
        }

        if (broadcast->getEntityType() == ITEM_TYPE) {
            broadcast_copy = new ItemBroadcast(*((ItemBroadcast*)broadcast));
        } else {
            broadcast_copy =
                new EntityBroadcast(*((EntityBroadcast*)broadcast));
        }
        it->second.push(broadcast_copy);
        ++it;
    }

    delete broadcast;
}

void ActiveClients::sendMessageToAll(Notification* message,
                                     const InstanceId sender) {
    auto it = content.begin();

    Notification* message_copy;

    while (it != this->content.end()) {
        if (it->first == sender) {
            ++it;
            continue;
        }

        message_copy = new Message(*((Message*)message));
        it->second.push(message_copy);
        ++it;
    }

    delete message;
}

void ActiveClients::stop() {
    for (auto it = content.begin(); it != content.end(); it++) {
        it->second.stop();
        it->second.join();
    }

    content.clear();
}

ActiveClients::~ActiveClients() {}

//-----------------------------------------------------------------------------
