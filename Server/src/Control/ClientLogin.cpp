#include "../../includes/Control/ClientLogin.h"

//-----------------------------------------------------------------------------
// Métodos privados

void ClientLogin::_loginProxy() {
    bool connected = false;

    while (!connected) {
        try {
            database.signIn("mauro", "123");
            connected = true;
        } catch (const LoginException& e) {
            fprintf(stderr, "LOGIN_ERROR: %s\n", e.what());
        }
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ClientLogin::ClientLogin(SocketWrapper& peer, Database& database,
                         Queue<NewConnection*>& new_connections)
    : is_running(false),
      peer(std::move(peer)),
      database(database),
      new_connections(new_connections) {}

void ClientLogin::run() {
    is_running = true;

    try {
        // Comienza la ejecución del clientlogin
        fprintf(stderr, "Inicia la ejecución del clientlogin.\n");

        // Loggear al usuario o crear al usuario
        // por ahora proxy:
        _loginProxy();

        // En este punto, vamos a tener la data del jugador conectado, por lo
        // que agregamos la petición de crear jugador a la cola y terminamos

        // esta data deberia venir el login
        CharacterCfg init_data = {100, 201, ALIVE, 0, 5, 5, DOWN_ORIENTATION};
        new_connections.push(new NewConnection(peer, init_data));

        // ACA NO SE DEBERÍA HACER NADA QUE FALLE, PORQUE EL SOCKET YA NO
        // ES NUESTRO, ASI QUE OJO

        // Finaliza la ejecución
        fprintf(stderr, "Finaliza la ejecución del clientlogin.\n");
    } catch (const std::exception& e) {
        // Error grave
        peer.shutdown();
        peer.close();
        fprintf(stderr, "%s\n", e.what());
    } catch (...) {
        // Error desconocido
        peer.shutdown();
        peer.close();
        fprintf(stderr, "Unknown error.\n");
    }

    is_running = false;
}

bool ClientLogin::isRunning() const {
    return is_running;
}

void ClientLogin::stop() {
    peer.shutdown();
    peer.close();
}

ClientLogin::~ClientLogin() {}

//-----------------------------------------------------------------------------
