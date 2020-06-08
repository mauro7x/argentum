#include "../../includes/View/Predictor.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

Predictor::Predictor(const MapView& map) : map(map) {}

bool Predictor::canMoveTo(const int x, const int y) const {
    if (map.collision(x, y)) {
        return false;
    }

    /* Hay que agregar chequeo de colisiones con NPCs y con otros PJ */

    return true;
}

Predictor::~Predictor() {}

//-----------------------------------------------------------------------------
