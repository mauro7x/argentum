

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
    next_instance_id(FIRST_INSTANCE_ID) {}

Game::~Game() { 
    // PERSISTIR TODO ANTES QUE SE DESTRUYA 
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
const int Game::newCharacter(CharacterPersistenceCfg& init_data) {

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



