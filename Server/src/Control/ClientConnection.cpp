#include "../../includes/Control/ClientConnection.h"
//-----------------------------------------------------------------------------
// Métodos privados

void ClientConnection::_finishThread() {
    std::unique_lock<std::mutex> l(m);
    // fprintf(stderr, "CLIENTE %i: se llamó a _finishThread.\n", id);
    if ((++finished_threads) == 2) {
        //fprintf(
            //stderr,
            //"CLIENTE %i: _finishThread agrega conexión terminada a la cola.\n",
            //id);
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
    // fprintf(stderr, "CLIENTE %i: Sender finaliza su ejecución.\n", id);

    try {
        Notification* notification = nullptr;
        bool socket_valid = true;
        while ((notification = notifications.pop())) {
            if (!notification->isForEveryMap()) {
                if (!(notification->getMapId() == this->map)) {
                    if (notification->isEntityBroadcast()) {
                        if (notification->getSourceInstanceId() != this->id) {
                            delete notification;
                            continue;
                        }
                    } else {
                        delete notification;
                        continue;
                    }
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
    // fprintf(stderr, "CLIENTE %i: Sender finaliza su ejecución.\n", id);
}

void ClientConnection::_receiver() {
    // ejecución del receiver loop
    // fprintf(stderr, "CLIENTE %i: Receiver comienza su ejecución.\n", id);

    try {
        uint8_t opcode;
        while (peer >> opcode) {
            switch (opcode) {
                case COMMAND_OPCODE: {
                    _receiveCommand();
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
    // fprintf(stderr, "CLIENTE %i: Receiver finaliza su ejecución.\n", id);
}

void ClientConnection::_receiveCommand() {
    uint8_t opcode_cmd;
    peer >> opcode_cmd;
    try {
        Command* cmd = CommandFactory::newCommand(id, opcode_cmd, peer);
        commands.push(cmd);
    } catch (const UnknownCommandException& e) {
        // Comando desconocido. Envio error.
        Reply* reply_error = new Reply(ERROR_MSG, e.what());
        this->notifications.push(reply_error);
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

    try {
        peer.shutdown();
    } catch (const Exception& e) {
        fprintf(stderr, "CLIENTE %i: error in socket::shutdown. Aborting.\n",
                id);
    }
}

void ClientConnection::changeMap(Id map) {
    this->map = map;
}

void ClientConnection::stop() {
    // Para detener al sender
    notifications.close();

    // Para detener al receiver
    try {
        peer.shutdown();
    } catch (const Exception& e) {
        fprintf(stderr, "CLIENTE %i: error in socket::shutdown. Aborting.\n",
                id);
    }
}

ClientConnection::~ClientConnection() {
    _freeNotifications();
}

//-----------------------------------------------------------------------------
