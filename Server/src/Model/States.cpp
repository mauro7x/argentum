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
        return new Alive(race_data.head_id, race_data.body_id);
    else if (state == DEAD)
        return new Dead(race_data.dead_body_id, race_data.dead_body_id);
    else
        throw Exception("State::newState invalid StateType.");
}

Alive::Alive(const Id head_id, const Id body_id) : State(head_id, body_id) {}

Alive::~Alive() {}

const bool Alive::attack() const {
    // Puede atacar
    return true;
}

const bool Alive::beAttacked() const {
    // Puede ser atacado
    return true;
}

const bool Alive::takeItem() const {
    // Puede tomar items
    return true;
}

const bool Alive::gatherGold() const {
    // Puede usar su oro
    return true;
}

const bool Alive::move() const {
    // Puede moverse
    return true;
}

const bool Alive::beHealed() const {
    // Puede ser curado
    return true;
}

const bool Alive::resurrect() const {
    // No puede resucitar. Ya está vivo.
    return false;
}

const bool Alive::meditate() const {
    // Puede meditar
    return true;
}

Dead::Dead(const Id dead_head_id, const Id dead_body_id)
    : State(dead_head_id, dead_body_id) {}

Dead::~Dead() {}

const bool Dead::attack() const {
    // No puede atacar
    return false;
}

const bool Dead::beAttacked() const {
    // No puede ser atacado
    return false;
}

const bool Dead::takeItem() const {
    // No puede tomar items
    return false;
}

const bool Dead::gatherGold() const {
    // No puede usar su oro
    return false;
}

const bool Dead::move() const {
    // Puede moverse
    return true;
}

const bool Dead::beHealed() const {
    // No puede ser curado
    return false;
}

const bool Dead::resurrect() const {
    // Puede resucitar
    return true;
}

const bool Dead::meditate() const {
    // No puede meditar
    return false;
}

Resurrecting::Resurrecting(const Id dead_head_id, const Id dead_body_id)
    : State(dead_head_id, dead_body_id) {}

Resurrecting::~Resurrecting() {}

const bool Resurrecting::attack() const {
    // No puede atacar
    return false;
}

const bool Resurrecting::beAttacked() const {
    // No puede ser atacado
    return false;
}

const bool Resurrecting::takeItem() const {
    // No puede tomar items
    return false;
}

const bool Resurrecting::gatherGold() const {
    // No puede usar su oro
    return false;
}

const bool Resurrecting::move() const {
    // No puede moverse
    return false;
}

const bool Resurrecting::beHealed() const {
    // No puede ser curado
    return false;
}

const bool Resurrecting::resurrect() const {
    // Ya está resucitando
    return true;
}

const bool Resurrecting::meditate() const {
    // No puede meditar
    return false;
}

//-----------------------------------------------------------------------------
