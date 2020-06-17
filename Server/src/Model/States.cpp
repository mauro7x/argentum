#include "../../includes/Model/States.h"

State::State() {}
State::~State() {}

Alive::Alive() {}
Alive::~Alive() {}

const bool Alive::canAttack() const {
    return true;
}

const bool Alive::canBeAttacked() const {
    return true;
}

Dead::Dead() {}
Dead::~Dead() {}

const bool Dead::canAttack() const {
    return false;
}

const bool Dead::canBeAttacked() const {
    return false;
}

