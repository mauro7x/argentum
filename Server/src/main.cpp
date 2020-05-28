// SERVIDOR

// ----------------------------------------------------------------------------
#include <stdio.h>

#include <string>

#include "../../Common/includes/Exception.h"
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
        // Código de ejecución

    } catch (const Exception& e) {
        fprintf(stderr, "%s\n", e.what());
        return ERROR;
    } catch (...) {
        fprintf(stderr, "Unknown error\n");
        return ERROR;
    }

    return SUCCESS;
}

// ----------------------------------------------------------------------------
