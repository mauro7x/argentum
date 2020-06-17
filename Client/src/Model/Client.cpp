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
        while ((command = commands.pop())) {
            delete command;
        }
    }

    {
        Update* update = NULL;
        while ((update = updates.pop())) {
            delete update;
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

    // una vez que tenemos el socket, lanzamos a los 3 hilos

    // FALTA VER COMO MANEJAR EL PRIMER PAQUETE (debería ser bloqueante)
    // UNA IDEA ES TENER UN MINI LOOP HASTA RECIBIRLO, Y AHI LAUNCHEAR LA VISTA.
    // ESTE LOOP PUEDE ESTAR DENTRO O FUERA DE LA VISTA.

    // Dispatcher y Updater (objetos activos)
    CommandDispatcher command_dispatcher(socket, commands, exit);
    Updater updater(socket, updates, exit);

    // Lanzamos los hilos
    command_dispatcher.start();
    updater.start();

    // FALTA VER COMO HACER QUE SI EL SOCKET SE CIERRA (DEL LADO DEL SERVER), LA
    // VISTA TERMINE

    // Lanzamos la vista del juego
    GameView game(commands, updates, exit);
    game();

    // Cerramos la conexión ordenadamente
    socket.shutdown();
    socket.close();

    // Cerramos la cola de comandos, y joineamos los hilos
    commands.close();
    command_dispatcher.join();
    updater.join();

    // En caso de que hayan quedado comandos o updates sin procesar.
    _freeQueues();

    fprintf(stderr, "DEBUG: Termina la ejecución del cliente.\n");
}

Client::~Client() {}

//-----------------------------------------------------------------------------
