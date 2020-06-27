#include "../includes/HomeView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void HomeView::_init() {
    // init
}

void HomeView::_loadMedia() {
    // init
}

void HomeView::_func(const int it) {
    // Proxy por ahora
    std::string hostname, port, try_again;
    bool connected = false;

    std::cout << "Bienvenido a Argentum Online.\n";

    while (!connected) {
        std::cout << "\n> Ingrese los datos que se le soliciten para "
                     "conectarse al "
                     "servidor.\n";
        std::cout << "\t> Hostname: ";
        std::getline(std::cin, hostname);
        std::cout << "\t> Puerto: ";
        std::getline(std::cin, port);

        std::cout << "[DEBUG] Hostname ingresado: " << hostname << "\n";
        std::cout << "[DEBUG] Puerto ingresado: " << port << "\n";

        try {
            socket = std::move(SocketWrapper(hostname, port));
            connected = true;
        } catch (const Exception& e) {
            std::cout << "\n> Conexión fallida. Ingrese 'y' para volver a "
                         "intentar, o presione enter para salir: \n";
            std::getline(std::cin, try_again);
            if (try_again != "y") {
                current_context = EXIT_CTX;
                quit();
                break;
            }
        }
    }

    if (connected) {
        current_context = CONNECTION_CTX;
        quit();
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

HomeView::HomeView(Context& current_context, SocketWrapper& socket)
    : ConstantRateFunc(RATE), current_context(current_context), socket(socket) {
    _init();
    _loadMedia();
}

HomeView::~HomeView() {}

//-----------------------------------------------------------------------------
