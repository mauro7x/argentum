#include "ParticularEntity.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ParticularEntity::ParticularEntity(SDL_Renderer* renderer) : Entity(renderer) {}

void ParticularEntity::loadMedia() {}

void ParticularEntity::handleEvent(const SDL_Event& e) {}

void ParticularEntity::act() {}

void ParticularEntity::render() const {}

void ParticularEntity::free() {}

ParticularEntity::~ParticularEntity() {}

//-----------------------------------------------------------------------------
