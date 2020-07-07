#include <stdio.h>

#include "Exception.h"
#include "View.h"

int main(int argc, char* argv[]) {
    try {
        View view;
        view();
    } catch (const Exception& e) {
        fprintf(stderr, "%s\n", e.what());
        return 1;
    } catch (...) {
        fprintf(stderr, "Error desconocido.\n");
        return 2;
    }

    return 0;
}