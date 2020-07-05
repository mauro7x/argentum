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

void Alive::attack() const {
    // Puede atacar
}

void Alive::beAttacked() const {
    // Puede ser atacacdo
}

void Alive::takeItem() const {
    // Puede tomar items
}

void Alive::gatherGold() const {
    // Puede usar su oro
}

void Alive::move() const {
    // Puede moverse
}

void Alive::beHealed() const {
    // Puede ser curado
}

void Alive::resurrect() const {
    // No puede resucitar. Ya está vivo.
    throw StateCantResurrectException();
}

void Alive::meditate() const {
    // Puede meditar
}

Dead::Dead(const Id dead_head_id, const Id dead_body_id)
    : State(dead_head_id, dead_body_id) {}

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

void Dead::gatherGold() const {
    // No puede usar su oro
    throw StateCantGatherGoldException();
}

void Dead::move() const {
    // Puede moverse
}

void Dead::beHealed() const {
    // No puede ser curado
    throw StateCantBeHealedException();
}

void Dead::resurrect() const {
    // Puede resucitar
}

void Dead::meditate() const {
    // No puede meditar
    throw StateCantMeditateException();
}

Resurrecting::Resurrecting(const Id dead_head_id, const Id dead_body_id)
    : State(dead_head_id, dead_body_id) {}

Resurrecting::~Resurrecting() {}

void Resurrecting::attack() const {
    // No puede atacar
    throw AttackerStateCantAttackException();
}

void Resurrecting::beAttacked() const {
    // No puede ser atacado
    throw AttackedStateCantBeAttackedException();
}

void Resurrecting::takeItem() const {
    // No puede tomar items
    throw StateCantTakeItemException();
}

void Resurrecting::gatherGold() const {
    // No puede usar su oro
    throw StateCantGatherGoldException();
}

void Resurrecting::move() const {
    // No puede moverse
    throw StateCantMoveException();
}

void Resurrecting::beHealed() const {
    // No puede ser curado
    throw StateCantBeHealedException();
}

void Resurrecting::resurrect() const {
    // Ya está resucitando
}

void Resurrecting::meditate() const {
    // No puede meditar
    throw StateCantMeditateException();
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

const char* StateCantGatherGoldException::what() const noexcept {
    return "No puedes usar tu oro. Estás muerto.";
}

const char* StateCantMoveException::what() const noexcept {
    return "No puedes moverte. Estás resucitando.";
}

const char* StateCantBeHealedException::what() const noexcept {
    return "No puedes curarte. Estás muerto.";
}

const char* StateCantResurrectException::what() const noexcept {
    return "No puedes resucitar. Ya estás vivo.";
}

const char* StateCantMeditateException::what() const noexcept {
    return "No puedes meditar. Estás muerto.";
}
