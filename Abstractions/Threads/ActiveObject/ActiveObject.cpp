#include "ActiveObject.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ActiveObject::ActiveObject() : is_running(false) {}

void ActiveObject::run() {
    is_running = true;
    // código que se ejecutará con el hilo

    is_running = false;
}

bool ActiveObject::isRunning() {
    return is_running;
}

ActiveObject::~ActiveObject() {}

//-----------------------------------------------------------------------------
