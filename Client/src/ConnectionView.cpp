#include "../includes/ConnectionView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void ConnectionView::_func(const int it) {
    // Proxy
    current_context = GAME_CTX;
    quit();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ConnectionView::ConnectionView(Context& current_context,
                               const SocketWrapper& socket)
    : ConstantRateFunc(RATE),
      current_context(current_context),
      socket(socket) {}

ConnectionView::~ConnectionView() {}

//-----------------------------------------------------------------------------
