#include "../../includes/Model/Client.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Client::Client() {}

void Client::run() {
    fprintf(stderr, "DEBUG: Comienza la ejecución del cliente\n");

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

    /* 2. LogInView */

    /* 3. Game view */
    GameView view;
    view();

    fprintf(stderr, "DEBUG: Termina la ejecución del cliente\n");
}

Client::~Client() {}

//-----------------------------------------------------------------------------
