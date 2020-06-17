#include "../../includes/Model/Updater.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Updater::Updater(const SocketWrapper& socket,
                 NonBlockingQueue<Update*>& updates, std::atomic_bool& exit)
    : socket(socket), updates(updates), exit(exit) {}

void Updater::run() {
    try {
        char opcode;
        while (socket >> opcode) {
            switch (opcode) {
                    // Identificar update, terminar de recibirlo, crearlo e
                    // insertarlo en la cola.

                default: {
                    // print proxy por ahora
                    fprintf(stderr, "Se recibió el opcode: %d\n", opcode);

                    /*
                    throw Exception(
                        "Unknown opcode received by client.");
                        */
                }
            }
        }
    } catch (const std::exception& e) {
        fprintf(stderr, "Updater // %s\n", e.what());
    } catch (...) {
        fprintf(stderr, "Updater // Unknown error.\n");
    }

    // Avisamos que se cerró el socket y que hay que terminar
    exit = true;
}

Updater::~Updater() {}

//-----------------------------------------------------------------------------
