// ----------------------------------------------------------------------------
// SERVIDOR - ARGENTUM ONLINE - https://github.com/mauro7x/taller_final
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
#include <stdio.h>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
#include <exception>
#include <string>
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
#include "../includes/Control/Server.h"
#include "../includes/defs.h"
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    if (argc != EXPECTED_ARGC) {
        fprintf(stderr, "Usage: %s <port>\n", argv[NAME]);
        return USAGE_ERROR;
    }

    std::string port = argv[PORT];

    try {
        Server server(port, MAX_CLIENTS_QUEUED);
        server.run();

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
