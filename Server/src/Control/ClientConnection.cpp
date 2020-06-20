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

    /** LOOP DE SENDER DE NOTIFICATIONS
     * Implementar el loop principal de este hilo.
     * Tiene que enviar notificacioens al cliente de la siguiente forma:
     *
     * 1. Saca un Notification* de la cola de notificaciones (aun no está
     * implementada, es similar a comando sólo que en vez de saber ejecutarse,
     * saben enviarse).
     *
     * 2. Se llama a Notification->send(peer), es decir se le pasa el peer
     * socket para que la notificación se envíe a si misma.
     *
     * 3. Se libera la notificacion haciendo delete Notification*.
     *
     * 4. Se repite desde 1.
     *
     * Deberá salir del loop cuando el socket peer deje de funcionar, ya sea
     * porque fue cerrado por el server o por el cliente.
     */

    try {
        Notification* notification = NULL;
        bool socket_valid = true;
        while ((notification = notifications.pop())) {
            socket_valid = notification->send(peer);
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

    // Proxy receptor de ProxyCommands.
    {
        while (1) {
            char cmd;
            this->peer.recv(&cmd, 1);
            fprintf(stderr, "ClientConnection: command %c received\n", cmd);
            if (cmd == 'e')
                break;
            CommandProxy* command = new CommandProxy(this->id, cmd);
            this->commands.push(command);
        }
    }

    /** LOOP DE RECEIVER DE COMANDOS
     * Implementar el loop principal de este hilo.
     * Tiene que recibir comandos desde el cliente de la siguiente forma:
     *
     * 1. Se recibe 1 byte de opcode.
     *
     * 2. Se switchea con el opcode y se recibe el resto del comando.
     *
     * 3. Se crea el comando en memoria dinámica y se lo almacena como un
     * Command* (de manera polimorfica) para luego pushearlo a la cola de
     * comandos `commands`.
     *
     * 4. Se repite desde 1.
     *
     * Deberá salir del loop cuando el receive del opcode devuelva 0 (es decir,
     * que se recibieron 0 bytes).
     */

    try {
        char opcode;
        while (peer >> opcode) {
            switch (opcode) {
                    // Identificar comando, terminar de recibirlo, crearlo e
                    // insertarlo en la cola.
                case (START_MOVING_UP_CMD || START_MOVING_DOWN_CMD ||
                    START_MOVING_LEFT_CMD || START_MOVING_RIGHT_CMD ||
                    STOP_MOVING_CMD): {
                    Command* command =
                        new CommandMovement(this->id, opcode, notifications);
                    break;
                }
                default: {
                    // print proxy por ahora
                    fprintf(stderr, "Se recibió el opcode: %d\n", opcode);

                    /*
                    throw Exception(
                        "Unknown opcode received by client handler.");
                        */
                }
            }
        }
    } catch (const std::exception& e) {
        // Error inesperado
        stop();
        fprintf(stderr, "ClientConnection // _receiver: %s\n", e.what());
    } catch (...) {
        // Error desconocido
        stop();
        fprintf(stderr, "ClientConnection // _receiver: Unknown error.\n");
    }

    // Avisamos que terminamos
    _finishThread();
    fprintf(stderr, "RECEIVER DE UN CLIENTE TERMINANDO! Id: %i\n", id);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ClientConnection::ClientConnection(
    const InstanceId id, SocketWrapper& peer,
    NonBlockingQueue<InstanceId*>& finished_connections,
    NonBlockingQueue<Command*>& commands)
    : id(id),
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
