// CLIENTE

// ----------------------------------------------------------------------------
#include <stdio.h>

#include <exception>

#include "../../includes/Model/Client.h"
#include "../../../Abstractions/Client-Server/Socket/SocketWrapper.h"
#include "../../includes/Model/defs.h"
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    if (argc != EXPECTED_ARGC) {
        fprintf(stderr, "Usage: %s\n", argv[NAME]);
        return USAGE_ERROR;
    }


    try {
        /* Use este codigo para testear cosas del server
        SocketWrapper sock("0", "11460");
        sock << "Test";
        */
        Client client;
        client.run();
    } catch (const std::exception& e) {
        fprintf(stderr, "%s\n", e.what());
        return ERROR;
    } catch (...) {
        fprintf(stderr, "Unknown error.\n");
        return ERROR;
    }

    return SUCCESS;
}

// ----------------------------------------------------------------------------
