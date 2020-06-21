#include "../../includes/Control/ClientConnection.h"
//-----------------------------------------------------------------------------
// Métodos privados

void ClientConnection::_finishThread() {
    std::unique_lock<std::mutex> l(m);
    fprintf(stderr, "Se llamó a finishThread...\n");
    if ((++finished_threads) == 2) {
        fprintf(stderr,
                "finishThread agregando conexión terminada a la cola...\n");
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
    fprintf(stderr, "SENDER DE UN CLIENTE EMPEZANDO! Id: %i\n", id);

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
    fprintf(stderr, "SENDER DE UN CLIENTE TERMINANDO! Id: %i\n", id);
}

void ClientConnection::_receiver() {
    // ejecución del receiver loop
    fprintf(stderr, "RECEIVER DE UN CLIENTE EMPEZANDO! Id: %i\n", id);

    try {
        uint8_t opcode;
        while (peer >> opcode) {
            switch (opcode) {
                case COMMAND_OPCODE: {
                    peer >> opcode;
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
    _finishThread();
    fprintf(stderr, "RECEIVER DE UN CLIENTE TERMINANDO! Id: %i\n", id);
}

void ClientConnection::_receiveCommand(char opcode) {
    Command* cmd = NULL;

    switch (opcode) {
        case START_MOVING_UP_CMD:
        case START_MOVING_DOWN_CMD:
        case START_MOVING_LEFT_CMD:
        case START_MOVING_RIGHT_CMD:
        case STOP_MOVING_CMD: {
            cmd = new CommandMovement(id, opcode);
            fprintf(stderr, "Command receiver: recibimos el comando %d\n",
                    opcode);
            commands.push(cmd);
            break;
        }

        default: {
            fprintf(stderr,
                    "ClientConnection::_receiveCommand: received %c (unknown "
                    "opcode).",
                    opcode);
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

    // peer.shutdown(); ESTA EXPLOTANDO, VER POR QUÉ
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
