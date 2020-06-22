#include <cstdio>  // debug
#include <tuple>
#include <utility>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Protocol.h"
//-----------------------------------------------------------------------------
#include "../../includes/Control/NotificationBroadcast.h"
#include "../../includes/Control/NotificationReply.h"
#include "../../includes/Control/ActiveClients.h"
//-----------------------------------------------------------------------------
#include "../../includes/Model/Game.h"
//-----------------------------------------------------------------------------
#define FIRST_INSTANCE_ID 1
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Game::Game(ActiveClients& active_clients)
    : next_instance_id(FIRST_INSTANCE_ID), active_clients(active_clients) {
    map_container.loadMaps();
}

Game::~Game() {
    // PERSISTIR TODO ANTES QUE SE DESTRUYA
}
//-----------------------------------------------------------------------------

Notification* Game::_buildBroadcast(InstanceId id,
                                             BroadcastType broadcast_type,
                                             EntityType entity_type) {
    PlayerData player_data;
    player_data.basic_data.gid = id;
    // llenar nickname
    player_data.nickname = "dummynick";
    Character& character = this->characters.at(id);
    character.fillBroadcastData(player_data);

    Notification* broadcast =
        new NotificationBroadcast(id, player_data, broadcast_type, entity_type);
    
    return broadcast;
}

void Game::_pushCharacterDifferentialBroadcast(InstanceId caller,
                                               BroadcastType broadcast_type,
                                               bool send_to_caller) {
    Notification* broadcast =
        _buildBroadcast(caller, broadcast_type, CHARACTER_TYPE);

    this->active_clients.sendDifferentialBroadcastToAll(broadcast, caller,
                                                        send_to_caller);

    this->characters.at(caller).beBroadcasted();
}

void Game::_pushFullBroadcast(InstanceId receiver, bool is_new_connection) {
    Notification* broadcast;

    if (is_new_connection) {
        // Si es una nueva conexión, envío al cliente nuevo su
        // NEW PLAYER BROADCAST
        broadcast = _buildBroadcast(receiver, NEW_BROADCAST, CHARACTER_TYPE);
        this->active_clients.notify(receiver, broadcast);
    }

    std::unordered_map<InstanceId, Character>::iterator it_characters =
        this->characters.begin();

    while (it_characters != this->characters.end()) {
        if (it_characters->first == receiver) {
            /*
             * El receptor no tiene nada que recibir. Si era una nueva conexion,
             * ya recibió su NEW PLAYER BROADCAST, y si cambia el mapa acá no
             * tiene que recibir sus propios datos.
             */
            ++it_characters;
            continue;
        }

        broadcast = _buildBroadcast(it_characters->first, NEW_BROADCAST,
                                    CHARACTER_TYPE);

        this->active_clients.notify(receiver, broadcast);

        ++it_characters;
    }
}

//-----------------------------------------------------------------------------
const int Game::newCharacter(CharacterCfg& init_data) {
    // CONTROL DE PLAYER DATA. LLENADO DE CAMPOS FALTANTES.
    // Establecer id del mapa y posicion en el mapa.
    // Como viene cargada la estructura?
    // La posicion en el mapa si es nuevo viene dada o la tengo que calcular?
    // Y si la posicion que tenia ahora esta ocupada?

    const Id spawning_map_id = this->map_container.getCharacterSpawningMap();
    int spawning_x_coord;
    int spawning_y_coord;
    this->map_container[spawning_map_id].establishCharacterSpawningPosition(
        this->next_instance_id, spawning_x_coord, spawning_y_coord);

    this->characters.emplace(
        std::piecewise_construct, std::forward_as_tuple(this->next_instance_id),
        std::forward_as_tuple(init_data, this->races[init_data.race],
                              this->kinds[init_data.kind], this->map_container,
                              spawning_map_id, spawning_x_coord,
                              spawning_y_coord));

    this->characters.at(next_instance_id).debug();

    ++this->next_instance_id;
    return next_instance_id - 1;
}

void Game::broadcastNewCharacter(InstanceId id) {
    _pushFullBroadcast(id, true);
    _pushCharacterDifferentialBroadcast(id, NEW_BROADCAST, false);
}

void Game::deleteCharacter(const InstanceId id) {
    if (!this->characters.count(id)) {
        throw Exception("deleteCharacter: Unknown character id [", id, "]");
    }

    _pushCharacterDifferentialBroadcast(id, DELETE_BROADCAST, false);

    // PERSISTIR ESTADO DEL JUGADOR

    // Lo eliminamos
    this->characters.erase(id);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Actualización del loop
//-----------------------------------------------------------------------------

void Game::actCharacters(const int it) {
    std::unordered_map<InstanceId, Character>::iterator it_characters =
        this->characters.begin();

    while (it_characters != this->characters.end()) {
        try {
            it_characters->second.act(it);
        } catch (const CollisionWhileMovingException& e) {
            it_characters->second.stopMoving();
            Notification* reply = new NotificationReply(ERROR_REPLY, e.what());
            active_clients.notify(it_characters->first, reply);
        }

        if (it_characters->second.mustBeBroadcasted()) {
            _pushCharacterDifferentialBroadcast(it_characters->first,
                                                UPDATE_BROADCAST, true);
        }

        it_characters->second.debug();

        ++it_characters;
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void Game::startMovingUp(const Id caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game.cpp startMovingUp: unknown caller.");
    }

    // fprintf(stderr, "GAME: Moving Up cmd\n");

    Character& character = this->characters.at(caller);

    try {
        character.startMovingUp();
    } catch (const CollisionWhileMovingException& e) {
        character.stopMoving();
        Notification* reply = new NotificationReply(ERROR_REPLY, e.what());
        active_clients.notify(caller, reply);
    }
}

void Game::startMovingDown(const Id caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game.cpp startMovingDown: unknown caller.");
    }

    // fprintf(stderr, "GAME: Moving Down cmd\n");

    Character& character = this->characters.at(caller);

    try {
        character.startMovingDown();
    } catch (const CollisionWhileMovingException& e) {
        character.stopMoving();
        Notification* reply = new NotificationReply(ERROR_REPLY, e.what());
        active_clients.notify(caller, reply);
    }
}

void Game::startMovingLeft(const Id caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game.cpp startMovingLeft: unknown caller.");
    }

    // fprintf(stderr, "GAME: Moving Left cmd\n");

    Character& character = this->characters.at(caller);

    try {
        character.startMovingLeft();
    } catch (const CollisionWhileMovingException& e) {
        character.stopMoving();
        Notification* reply = new NotificationReply(ERROR_REPLY, e.what());
        active_clients.notify(caller, reply);
    }
}

void Game::startMovingRight(const Id caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game.cpp startMovingRight: unknown caller.");
    }

    // fprintf(stderr, "GAME: Moving Right cmd\n");

    Character& character = this->characters.at(caller);

    try {
        character.startMovingRight();
    } catch (const CollisionWhileMovingException& e) {
        character.stopMoving();
        Notification* reply = new NotificationReply(ERROR_REPLY, e.what());
        active_clients.notify(caller, reply);
    }
}

void Game::stopMoving(const Id caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game.cpp stopMoving: unknown caller.");
    }

    // fprintf(stderr, "GAME: Stop Moving cmd\n");

    Character& character = this->characters.at(caller);
    character.stopMoving();
}
//-----------------------------------------------------------------------
const Id Game::getMapId(const InstanceId caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game.cpp::getMapId: unknown caller.");
    }

    return this->characters.at(caller).getMapId();
}
