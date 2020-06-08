#include "../includes/States.h"

State::State() {}

Alive::Alive() {}
Alive::~Alive() {}

const bool Alive::canAttack() const {
    return true;
}

Dead::Dead() {}
Dead::~Dead() {}

const bool Dead::canAttack() const {
    return false;
}

//Extender comportamiento segun necesidad
