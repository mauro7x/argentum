#include "../../includes/Model/LogIn.h"

//-----------------------------------------------------------------------------
// Métodos privados

void LogInProxy::_receiveFirstPackage() const {
    uint8_t opcode = 0, entity_type = 0;
    size_t received;

    // Recibimos el primer paquete según el procolo:
    // OP (1) - BROADCAST_OP (1) - ENTITY_TYPE (1) - LENGTH (4) - DATA (LENGTH)

    received = (socket >> opcode);
    if (!received) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: incomplete first package data "
            "(socket was closed).");
    } else if (opcode != BROADCAST_OPCODE) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: received another package before "
            "the initial data.");
    }

    received = (socket >> opcode);
    if (!received) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: incomplete first package data "
            "(socket was closed).");
    } else if (opcode != NEW_BROADCAST) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: received another package before "
            "the initial data.");
    }

    received = (socket >> entity_type);
    if (!received) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: incomplete first package data "
            "(socket was closed).");
    } else if (entity_type != PLAYER_TYPE) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: received another package before "
            "the initial data.");
    }

    std::vector<uint8_t> serialized_data;
    received = (socket >> serialized_data);
    if (!received) {
        throw Exception(
            "LogInProxy::_receiveFirstPackage: incomplete first package data "
            "(socket was closed).");
    }

    PlayerData received_data;
    json j = json::from_msgpack(serialized_data);
    received_data = j.get<PlayerData>();
    broadcasts.push(new NewPlayerBroadcast(received_data));
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

LogInProxy::LogInProxy(SocketWrapper& socket,
                       NonBlockingQueue<Broadcast*>& broadcasts)
    : socket(socket), broadcasts(broadcasts) {}

void LogInProxy::operator()() const {
    /* Acá debe ir toda la lógica del sign-in / sign-up. */

    /* Por ahora el server nos enviará la data apenas nos conectemos al mismo,
     * sin esta parte, por lo que la salteamos asumiendo que la próxima data que
     * nos llegue del mismo será la data inicial de nuestro jugador. */

    _receiveFirstPackage();
}

LogInProxy::~LogInProxy() {}

//-----------------------------------------------------------------------------
