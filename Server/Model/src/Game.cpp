

#include "../../../Common/includes/Exceptions/Exception.h"
//-----------------------------------------------------------------------------
#include "../includes/Game.h"
//-----------------------------------------------------------------------------
#define FIRST_INSTANCE_ID 1
#define ID_MAP_CHARACTER_SPAWN 1
#define INIT_X_COORD_CHARACTER_SPAWN 1
#define INIT_Y_COORD_CHARACTER_SPAWN 1
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Game::Game():
    next_instance_id(FIRST_INSTANCE_ID) {}

Game::~Game() { 
    // PERSISTIR TODO ANTES QUE SE DESTRUYA 
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
const int Game::newCharacter(const int id_race, const int id_kind) {
    this->characters.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(this->next_instance_id), 
        std::forward_as_tuple(
            this->races[id_race], this->kinds[id_kind], 
            ID_MAP_CHARACTER_SPAWN, INIT_X_COORD_CHARACTER_SPAWN, 
            INIT_Y_COORD_CHARACTER_SPAWN, this->map_container));

    // CAMBIAR VALORES DE POSITION HARDCODEADOS EN CREACION DE CHARACTER
    
    ++this->next_instance_id;
    return next_instance_id - 1;
}

void Game::deleteCharacter(const int id) {
    if (!this->characters.count(id)) {
        throw Exception("deleteCharacter: Unknown character id [", id, "]");
    }

    // PERSISTIR ESTADO DEL JUGADOR

    this->characters.erase(id);
}
//-----------------------------------------------------------------------------



