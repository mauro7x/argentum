#include "../../includes/Model/States.h"

#include "../../../Common/includes/Exceptions/Exception.h"

State::State() {}
State::~State() {}

State* StateFactory::newState(StateType state) {
    if (state == ALIVE)
        return new Alive();
    else if (state == DEAD)
        return new Dead();
    else
        throw Exception("State::newState invalid StateType.");
}

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
