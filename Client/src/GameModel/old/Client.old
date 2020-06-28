#include "../../includes/Model/Client.h"

//-----------------------------------------------------------------------------
// Métodos privados

bool Client::_connect(SocketWrapper& socket) const {
    std::string hostname, port, try_again;
    bool connected = false;

    std::cout << "Bienvenido a Argentum Online.\n";

    while (!connected) {
        std::cout
            << "\n> Ingrese los datos que se le soliciten para conectarse al "
               "servidor.\n";
        std::cout << "\t> Hostname: ";
        std::getline(std::cin, hostname);
        std::cout << "\t> Puerto: ";
        std::getline(std::cin, port);

        std::cout << "[DEBUG] Hostname ingresado: " << hostname << "\n";
        std::cout << "[DEBUG] Puerto ingresado: " << port << "\n";

        try {
            socket = std::move(SocketWrapper(hostname, port));
            connected = true;
        } catch (const Exception& e) {
            std::cout << "\n> Conexión fallida. Ingrese 'y' para volver a "
                         "intentar, o presione enter para salir: \n";
            std::getline(std::cin, try_again);
            if (try_again != "y") {
                return false;
            }
        }
    }

    return true;
}

void Client::_finish(SocketWrapper& socket,
                     CommandDispatcher& command_dispatcher,
                     Receiver& receiver) {
    // Cerramos la cola de comandos para que el dispatcher termine
    commands.close();

    // Cerramos la conexión ordenadamente
    socket.shutdown();

    // Joineamos los hilos
    command_dispatcher.join();
    receiver.join();

    // En caso de que las colas no hayan sido vaciadas
    _freeQueues();
}

void Client::_freeQueues() {
    {
        Command* command = NULL;
        commands.close();
        while ((command = commands.pop())) {
            delete command;
        }
    }

    {
        Broadcast* broadcast = NULL;
        while ((broadcast = broadcasts.pop())) {
            delete broadcast;
        }
    }

    {
        Message* message = NULL;
        while ((message = messages.pop())) {
            delete message;
        }
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Client::Client() : exit(false) {}

void Client::run() {
    fprintf(stderr, "DEBUG: Comienza la ejecución del cliente.\n");

    // login proxy (va a ser una vista)
    SocketWrapper socket;
    if (!_connect(socket)) {
        fprintf(stderr,
                "DEBUG: Terminamos el cliente sin habernos conectado al "
                "servidor.\n");
        return;
    }

    // Ahora estamos conectados con el server, debemos loggearnos o crear un
    // nuevo personaje

    // Proxy para el LogIn
    {
        LogInProxy login(socket, broadcasts);
        login();
    }

    // Ahora ya nos encontramos conectados y recibimos la data del primer
    // paquete. Lanzamos los hilos

    // Dispatcher y Receiver (objetos activos)
    CommandDispatcher command_dispatcher(socket, commands, exit);
    Receiver receiver(socket, broadcasts, messages, exit);

    command_dispatcher.start();
    receiver.start();

    try {
        // Lanzamos la vista del juego
        GameView game(commands, broadcasts, messages, exit);
        game();

    } catch (const Exception& e) {
        _finish(socket, command_dispatcher, receiver);
        throw e;
    } catch (const std::exception& e) {
        _finish(socket, command_dispatcher, receiver);
        throw e;
    } catch (...) {
        _finish(socket, command_dispatcher, receiver);
        throw;
    }

    // Terminamos la ejecución ordenadamente
    _finish(socket, command_dispatcher, receiver);

    fprintf(stderr, "DEBUG: Termina la ejecución del cliente.\n");
}

Client::~Client() {
    // Nos aseguramos de no perder memoria en las colas
    _freeQueues();
}

//-----------------------------------------------------------------------------
