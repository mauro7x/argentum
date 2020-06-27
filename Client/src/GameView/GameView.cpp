#include "../../includes/GameView/GameView.h"

//-----------------------------------------------------------------------------
// Métodos privados

void GameView::_func(const int it) {
    // Proxy
    quit();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

GameView::GameView(BlockingQueue<Command*>& commands,
                   NonBlockingQueue<Broadcast*>& broadcasts,
                   NonBlockingQueue<Message*>& messages, Renderer& renderer)
    : ConstantRateFunc(RATE),
      commands(commands),
      broadcasts(broadcasts),
      messages(messages),
      renderer(renderer) {}

GameView::~GameView() {}

//-----------------------------------------------------------------------------
