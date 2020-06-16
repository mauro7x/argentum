#include "../../includes/Model/Client.h"

#include <iostream>
#include <string>

#include "../../../Common/includes/Socket/SocketWrapper.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Client::Client() {}

void Client::run() {
    fprintf(stderr, "DEBUG: Comienza la ejecución del cliente\n");

    /* Conexión proxy

    std::string host, port;
    bool connect = false;
    SocketWrapper socket;
    while(!connect){
        std::cout << "ingrese host: " << std::endl;
        std::cin >> host;
        std::cout << "ingrese port: " << std::endl;
        std::cin >> port;
        try {
            socket = std::move(SocketWrapper(host, port));

        } catch (const Exception& e) {
            std::cout << "host o/y port ingresado invalido " << std::endl;
            continue;
        }
        connect = true;
        std::cout << "conexion establicida!" << std::endl;
    }
    */

    /**
     * La idea es la siguiente: vamos a presentar 3 pantallas independientes al
     * cliente en el siguiente orden:
     *
     * 1. ServerConnectionView: se pedira una ip y un puerto para conectarse.
     * Cuando se logre la conexión, se retornará el socket conectado y se
     * cerrará la ventana.
     *
     * 2. LogInView: con el socket conectado, esta pantalla buscará loggear al
     * jugador con usuario y contraseña, y le dará la opcion de crear un nuevo
     * jugador. Selección de personaje.
     *
     * 3. GameView: una vez que se seleccionó el personaje, se inicia el juego.
     */

    /* 1. ServerConnectionView */
    /* Una vez pasada esta view, contaremos con una conexión establecida. En
     * este momento, lanzamos los distintos hilos de comunicación con el
     * servidor: Sender, Receiver. Estos se pueden encapsular en una clase
     * Server. Por ahora, será un server proxy que no tendrá sender y receiver,
     * pero luego se cambiará manteniendo el código escrito en el gameview. */

    /* 2. LogInView */
    /* Pasada esta etapa, el juego comienza, y lo que debemos hacer es
     * renderizar lo que el server nos mande. */

    /* 3. Game view */
    GameView view;
    view();

    /* ENCAPSULAR TODO ESTO EN SCOPES, TENER EN CUENTA QUE LAS VISTAS DEBERIAN
     * DESTRUIRSE PARA LIBERAR LOS RECURSOS QUE OCUPAN (usamos RAII) */

    fprintf(stderr, "DEBUG: Termina la ejecución del cliente\n");
}

Client::~Client() {}

//-----------------------------------------------------------------------------
