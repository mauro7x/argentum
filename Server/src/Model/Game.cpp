#include <utility>
#include <tuple>

#include <cstdio> // debug

#include "../../../Common/includes/Exceptions/Exception.h"
//-----------------------------------------------------------------------------
#include "../../includes/Model/Game.h"
//-----------------------------------------------------------------------------
#define FIRST_INSTANCE_ID 1
#define ID_MAP_CHARACTER_SPAWN 1
#define INIT_X_COORD_CHARACTER_SPAWN 1
#define INIT_Y_COORD_CHARACTER_SPAWN 1
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Game::Game():
        next_instance_id(FIRST_INSTANCE_ID) {
    map_container.loadMaps();
}

Game::~Game() { 
    // PERSISTIR TODO ANTES QUE SE DESTRUYA 
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
const int Game::newCharacter(CharacterCfg& init_data) {

    // CONTROL DE PLAYER DATA. LLENADO DE CAMPOS FALTANTES.
    // Establecer id del mapa y posicion en el mapa.
    // Como viene cargada la estructura?
    // La posicion en el mapa si es nuevo viene dada o la tengo que calcular?
    // Y si la posicion que tenia ahora esta ocupada?

    this->characters.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(this->next_instance_id),
        std::forward_as_tuple(init_data, 
                              this->races[init_data.race], 
                              this->kinds[init_data.kind], 
                              this->map_container));
    
    this->characters.at(next_instance_id).debug();
    
    ++this->next_instance_id;
    return next_instance_id - 1;
}

void Game::deleteCharacter(const InstanceId id) {
    if (!this->characters.count(id)) {
        throw Exception("deleteCharacter: Unknown character id [", id, "]");
    }

    // PERSISTIR ESTADO DEL JUGADOR

    this->characters.erase(id);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Actualización del loop
//-----------------------------------------------------------------------------

void Game::actCharacters(const int it) {
    std::unordered_map<InstanceId, Character>::iterator it_characters = this->characters.begin();

    while (it_characters != this->characters.end()) {
        try {
            it_characters->second.act(it);
        } catch(const CollisionWhileMovingException& e) {
            // RESPONDER QUE NO SE PUEDE MOVER MAS.
            fprintf(stderr, "ACT EXCEPTION: NO SE PUEDE MOVER MAS\n");
        }
        it_characters->second.debug();

        ++it_characters;
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
NotificationReply Game::startMovingUp(const Id caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game.cpp startMovingUp: unknown caller.");
    }

    fprintf(stderr, "GAME: Moving Up cmd\n");

    Character& character = this->characters.at(caller);
    character.startMovingUp();
}

NotificationReply Game::startMovingDown(const Id caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game.cpp startMovingDown: unknown caller.");
    }

    fprintf(stderr, "GAME: Moving Down cmd\n");

    Character& character = this->characters.at(caller);
    character.startMovingDown();
}

NotificationReply Game::startMovingLeft(const Id caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game.cpp startMovingLeft: unknown caller.");
    }

    fprintf(stderr, "GAME: Moving Left cmd\n");
    
    Character& character = this->characters.at(caller);
    character.startMovingLeft();
}

NotificationReply Game::startMovingRight(const Id caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game.cpp startMovingRight: unknown caller.");
    }

    fprintf(stderr, "GAME: Moving Right cmd\n");

    Character& character = this->characters.at(caller);
    character.startMovingRight();
}

NotificationReply Game::stopMoving(const Id caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game.cpp stopMoving: unknown caller.");
    }

    fprintf(stderr, "GAME: Stop Moving cmd\n");
    
    Character& character = this->characters.at(caller);
    character.stopMoving();
}

