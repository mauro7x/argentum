// CLIENTE

// ----------------------------------------------------------------------------
#include <stdio.h>

#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../includes/Model/Client.h"
#include "../../includes/Model/defs.h"
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    if (argc != EXPECTED_ARGC) {
        fprintf(stderr, "Usage: %s\n", argv[NAME]);
        return USAGE_ERROR;
    }

    try {
        Client client;
        client.run();

    } catch (const Exception& e) {
        fprintf(stderr, "%s\n", e.what());
        return ERROR;
    } catch (...) {
        fprintf(stderr, "Unknown error.\n");
        return ERROR;
    }

    return SUCCESS;
}

// ----------------------------------------------------------------------------
