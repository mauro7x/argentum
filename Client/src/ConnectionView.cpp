#include "../includes/ConnectionView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void ConnectionView::_func(const int it) {
    fprintf(stderr, "Inicia ConnectionView.\n");

    // Proxy
    current_context = GAME_CTX;
    stop();

    fprintf(stderr, "Finaliza ConnectionView.\n");
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ConnectionView::ConnectionView(Context& current_context, SocketWrapper& socket)
    : ConstantRateFunc(RATE),
      current_context(current_context),
      socket(socket) {}

ConnectionView::~ConnectionView() {}

//-----------------------------------------------------------------------------
