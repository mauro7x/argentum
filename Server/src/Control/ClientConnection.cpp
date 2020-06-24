#include "../../includes/Control/ClientConnection.h"
//-----------------------------------------------------------------------------
// Métodos privados

void ClientConnection::_finishThread() {
    std::unique_lock<std::mutex> l(m);
    fprintf(stderr, "CLIENTE %i: se llamó a _finishThread.\n", id);
    if ((++finished_threads) == 2) {
        fprintf(
            stderr,
            "CLIENTE %i: _finishThread agrega conexión terminada a la cola.\n",
            id);
        finished_connections.push(new InstanceId(id));
    }
}

void ClientConnection::_freeNotifications() {
    notifications.close(); /* nos aseguramos de que no sea bloqueante */
    Notification* notification = NULL;
    while ((notification = notifications.pop())) {
        delete notification;
    }
}

void ClientConnection::_sender() {
    // ejecución del sender loop
    fprintf(stderr, "CLIENTE %i: Sender finaliza su ejecución.\n", id);

    try {
        Notification* notification = nullptr;
        bool socket_valid = true;
        while ((notification = notifications.pop())) {
            if (notification->isBroadcast()) {
                if (!(notification->getMapId() == this->map)) {
                    delete notification;
                    continue;
                }
            }
            socket_valid = notification->send(this->id, peer);
            delete notification;

            if (!socket_valid) {
                // Se cerró el socket y hay que terminar
                break;
            }
        }
    } catch (const std::exception& e) {
        // Error inesperado
        stop();
        fprintf(stderr, "ClientConnection // _sender: %s\n", e.what());
    } catch (...) {
        // Error desconocido
        stop();
        fprintf(stderr, "ClientConnection // _sender: Unknown error.\n");
    }

    // Avisamos que terminamos
    _finishThread();
    fprintf(stderr, "CLIENTE %i: Sender finaliza su ejecución.\n", id);
}

void ClientConnection::_receiver() {
    // ejecución del receiver loop
    fprintf(stderr, "CLIENTE %i: Receiver comienza su ejecución.\n", id);

    try {
        uint8_t opcode;
        while (peer >> opcode) {
            switch (opcode) {
                case COMMAND_OPCODE: {
                    size_t received = (peer >> opcode);
                    if (!received) {
                        throw Exception("Incomplete command received.");
                    }
                    _receiveCommand(opcode);
                    break;
                }

                default: {
                    throw Exception(
                        "Unknown opcode received by client handler.");
                }
            }
        }
    } catch (const std::exception& e) {
        // Error inesperado
        stop();
        fprintf(stderr, "ClientConnection::_receiver: %s\n", e.what());
    } catch (...) {
        // Error desconocido
        stop();
        fprintf(stderr, "ClientConnection::_receiver: Unknown error.\n");
    }

    // Avisamos que terminamos
    this->notifications.close();
    _finishThread();
    fprintf(stderr, "CLIENTE %i: Receiver finaliza su ejecución.\n", id);
}

void ClientConnection::_receiveCommand(uint8_t opcode) {
    Command* cmd = NULL;

    switch (opcode) {
        case START_MOVING_UP_CMD:
        case START_MOVING_DOWN_CMD:
        case START_MOVING_LEFT_CMD:
        case START_MOVING_RIGHT_CMD:
        case STOP_MOVING_CMD: {
            cmd = new CommandMovement(id, opcode);
            fprintf(stderr,
                    "CLIENTE %i: Se recibió el comando con opcode %d.\n", id,
                    opcode);
            commands.push(cmd);
            break;
        }

        default: {
            fprintf(stderr,
                    "CLIENTE %i: Se recibió el comando DESCONOCIDO con opcode "
                    "%d. Se ignora.\n",
                    id, opcode);
            break;
        }
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ClientConnection::ClientConnection(
    const InstanceId id, const Id map, SocketWrapper& peer,
    NonBlockingQueue<InstanceId*>& finished_connections,
    NonBlockingQueue<Command*>& commands)
    : id(id),
      map(map),
      peer(std::move(peer)),
      finished_connections(finished_connections),
      finished_threads(0),
      commands(commands) {}

void ClientConnection::start() {
    sender = std::thread(&ClientConnection::_receiver, this);
    receiver = std::thread(&ClientConnection::_sender, this);
}

void ClientConnection::push(Notification* notification) {
    notifications.push(notification);
}

void ClientConnection::join() {
    if (sender.joinable()) {
        sender.join();
    }

    if (receiver.joinable()) {
        receiver.join();
    }

    peer.shutdown();
}

void ClientConnection::changeMap(Id map) {
    this->map = map;
}

void ClientConnection::stop() {
    // Para detener al sender
    notifications.close();

    // Para detener al receiver
    peer.shutdown();
    peer.close();
}

ClientConnection::~ClientConnection() {
    _freeNotifications();
}

//-----------------------------------------------------------------------------
