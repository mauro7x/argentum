#include <iostream>

#include "Exception.h"
#include "SDLException.h"
#include "View.h"

int main(int argc, char* argv[]) {
    try {
        View view;
        view();
    } catch (const SDLException& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "SDL Error: " << e.sdlError() << std::endl;
        return 1;
    } catch (const Exception& e) {
        std::cerr << e.what() << std::endl;
        return 2;
    } catch (...) {
        std::cerr << "Error desconocido." << std::endl;
        return 3;
    }

    return 0;
}