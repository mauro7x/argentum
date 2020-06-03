#include "../../includes/Model/Client.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Client::Client() {}

void Client::run() {
    std::cout << "Comienza la ejecución del cliente." << std::endl;

    // Ejecución del cliente
    GameView view;
    view();

    std::cout << "Termina la ejecución del cliente." << std::endl;
}

Client::~Client() {}

//-----------------------------------------------------------------------------
