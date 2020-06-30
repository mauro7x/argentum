#include "../../includes/Model/States.h"

#include "../../../Common/includes/Exceptions/Exception.h"

State::State(Id head_id, Id body_id) : head_id(head_id), body_id(body_id) {}
State::~State() {}

void State::fillBroadcastData(PlayerData& data) {
    data.head_id = this->head_id;
    data.body_id = this->body_id;
}

void State::fillPersistenceData(CharacterCfg& data) {
    data.head_id = this->head_id;
    data.body_id = this->body_id;
}

State* StateFactory::newState(StateType state, const Race& race_data) {
    if (state == ALIVE)
        return new Alive(race_data);
    else if (state == DEAD)
        return new Dead(race_data);
    else
        throw Exception("State::newState invalid StateType.");
}

Alive::Alive(const Race& race_data)
    : State(race_data.head_id, race_data.body_id) {}

Alive::~Alive() {}

void Alive::attack() const {
    // Puede atacar
}

void Alive::beAttacked() const {
    // Puede ser atacacdo
}

void Alive::takeItem() const {
    // Puede tomar items
}

Dead::Dead(const Race& race_data)
    : State(race_data.dead_head_id, race_data.dead_body_id) {}

Dead::~Dead() {}

void Dead::attack() const {
    // No puede atacar
    throw AttackerStateCantAttackException();
}

void Dead::beAttacked() const {
    // No puede ser atacado
    throw AttackedStateCantBeAttackedException();
}

void Dead::takeItem() const {
    // No puede tomar items
    throw StateCantTakeItemException();
}

const char* AttackedStateCantBeAttackedException::what() const noexcept {
    return "No puedes atacar a ese jugador. Ya está muerto";
}

const char* AttackerStateCantAttackException::what() const noexcept {
    return "No puedes atacar. Estás muerto.";
}

const char* StateCantTakeItemException::what() const noexcept {
    return "No puedes tomar items. Estás muerto.";
}
