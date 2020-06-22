#include "../../includes/Model/Client.h"

#include <iostream>
#include <string>

#include "../../../Common/includes/Socket/SocketWrapper.h"

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
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Client::Client() : exit(false), first_package_received(false) {}

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

    // una vez que tenemos el socket, lanzamos a los 3 hilos

    // Dispatcher y Receiver (objetos activos)
    CommandDispatcher command_dispatcher(socket, commands, exit);
    Receiver receiver(socket, broadcasts, exit, first_package_received);

    // Lanzamos el receiver
    receiver.start();

    // Decirle al usuario que esta cargando

    // Dormimos hasta recibir el primer paquete
    {
        while (!first_package_received) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
  
     // Lanzamos el command dispatcher
    command_dispatcher.start();
  
    try {
        // Lanzamos la vista del juego
        GameView game(commands, updates, exit);
        game();

    } catch (const Exception& e) {
        socket.shutdown();
        socket.close();
        commands.close();
        command_dispatcher.join();
        updater.join();
        throw e;
    } catch (const std::exception& e) {
        socket.shutdown();
        socket.close();
        commands.close();
        command_dispatcher.join();
        updater.join();
        throw e;
    } catch (...) {
        socket.shutdown();
        socket.close();
        commands.close();
        command_dispatcher.join();
        updater.join();
        throw;
    }

    // Cerramos la conexión ordenadamente
    socket.shutdown();
    socket.close();

    // Cerramos la cola de comandos, y joineamos los hilos
    commands.close();
    command_dispatcher.join();
    receiver.join();

    // En caso de que hayan quedado comandos o broadcasts sin procesar.
    _freeQueues();

    fprintf(stderr, "DEBUG: Termina la ejecución del cliente.\n");
}

Client::~Client() {
    // Liberamos las colas
    _freeQueues();
}

//-----------------------------------------------------------------------------
