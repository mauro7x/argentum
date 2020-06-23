#include <cstdio>  // debug
#include <tuple>
#include <utility>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Protocol.h"
#include "../../../Common/includes/RandomNumberGenerator.h"
//-----------------------------------------------------------------------------
#include "../../includes/Control/ActiveClients.h"
#include "../../includes/Control/NotificationBroadcast.h"
#include "../../includes/Control/NotificationReply.h"
//-----------------------------------------------------------------------------
#include "../../includes/Model/Game.h"
//-----------------------------------------------------------------------------
#define FIRST_INSTANCE_ID 1
#define RATE 1000 / 30
#define MAX_CREATURES_PER_MAP 20
#define TIME_TO_SPAWN_CREATURE 3000  // en ms
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
Game::Game(ActiveClients& active_clients)
    : next_instance_id(FIRST_INSTANCE_ID),
      active_clients(active_clients),
      creature_spawn_cooldown(0) {
    map_container.loadMaps();

    std::vector<Id> maps_id = std::move(this->map_container.getMapsId());

    for (unsigned int i = 0; i < maps_id.size(); ++i) {
        this->maps_creatures_info.emplace(
            std::piecewise_construct, std::forward_as_tuple(maps_id[i]),
            std::forward_as_tuple(0, TIME_TO_SPAWN_CREATURE));
    }
}

Game::~Game() {
    // PERSISTIR TODO ANTES QUE SE DESTRUYA
}
//-----------------------------------------------------------------------------

MapCreaturesInfo::MapCreaturesInfo(unsigned int amount_of_creatures,
                                   int creature_spawning_cooldown)
    : amount_of_creatures(amount_of_creatures),
      creature_spawning_cooldown(creature_spawning_cooldown) {}

//-----------------------------------------------------------------------------

Notification* Game::_buildPlayerBroadcast(InstanceId id,
                                          BroadcastType broadcast_type) {
    PlayerData player_data;
    player_data.basic_data.gid = id;
    // llenar nickname
    player_data.nickname = "dummynick";
    Character& character = this->characters.at(id);
    character.fillBroadcastData(player_data);

    Notification* broadcast =
        new NotificationBroadcast(id, player_data, broadcast_type);

    return broadcast;
}

Notification* Game::_buildCreatureBroadcast(InstanceId id,
                                            BroadcastType broadcast_type) {
    CreatureData creature_data;
    creature_data.basic_data.gid = id;
    Creature& creature = this->creatures.at(id);
    creature.fillBroadcastData(creature_data);

    Notification* broadcast =
        new NotificationBroadcast(id, creature_data, broadcast_type);

    return broadcast;
}

void Game::_pushCharacterDifferentialBroadcast(InstanceId caller,
                                               BroadcastType broadcast_type,
                                               bool send_to_caller) {
    Notification* broadcast = _buildPlayerBroadcast(caller, broadcast_type);

    this->active_clients.sendDifferentialBroadcastToAll(broadcast, caller,
                                                        send_to_caller);

    this->characters.at(caller).beBroadcasted();
}

void Game::_pushCreatureDifferentialBroadcast(InstanceId creature,
                                              BroadcastType broadcast_type) {
    Notification* broadcast = _buildCreatureBroadcast(creature, broadcast_type);

    this->active_clients.sendDifferentialBroadcastToAll(broadcast, creature,
                                                        false);

    this->creatures.at(creature).beBroadcasted();
}

void Game::_pushFullBroadcast(InstanceId receiver, bool is_new_connection) {
    Notification* broadcast;

    if (is_new_connection) {
        // Si es una nueva conexión, envío al cliente nuevo su
        // NEW PLAYER BROADCAST
        broadcast = _buildPlayerBroadcast(receiver, NEW_BROADCAST);
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

        broadcast = _buildPlayerBroadcast(it_characters->first, NEW_BROADCAST);

        this->active_clients.notify(receiver, broadcast);

        ++it_characters;
    }

    std::unordered_map<InstanceId, Creature>::iterator it_creatures =
        this->creatures.begin();

    while (it_creatures != this->creatures.end()) {
        broadcast = _buildCreatureBroadcast(it_creatures->first, NEW_BROADCAST);

        this->active_clients.notify(receiver, broadcast);
        
        ++it_creatures;
    }
}

    //-----------------------------------------------------------------------------
    const InstanceId Game::newCharacter(const CharacterCfg& init_data) {
        // CONTROL DE PLAYER DATA. LLENADO DE CAMPOS FALTANTES.
        // Establecer id del mapa y posicion en el mapa.
        // Como viene cargada la estructura?
        // La posicion en el mapa si es nuevo viene dada o la tengo que
        // calcular? Y si la posicion que tenia ahora esta ocupada?

        Id new_character_id = this->next_instance_id;
        ++this->next_instance_id;

        const Id spawning_map_id =
            this->map_container.getCharacterSpawningMap();
        int spawning_x_coord;
        int spawning_y_coord;
        this->map_container[spawning_map_id].establishEntitySpawningPosition(
            new_character_id, spawning_x_coord, spawning_y_coord, false);

        this->characters.emplace(
            std::piecewise_construct, std::forward_as_tuple(new_character_id),
            std::forward_as_tuple(init_data, this->races[init_data.race],
                                  this->kinds[init_data.kind],
                                  this->map_container, spawning_map_id,
                                  spawning_x_coord, spawning_y_coord));

        this->characters.at(new_character_id).debug();

        return new_character_id;
    }

    void Game::newCreature(const CreatureCfg& init_data, const Id init_map) {
        int spawning_x_coord;
        int spawning_y_coord;
        this->map_container[init_map].establishEntitySpawningPosition(
            this->next_instance_id, spawning_x_coord, spawning_y_coord, true);

        // AGREGAR LOGICA DE INIT_HEALTH E INIT_DAMAGE EN BASE AL NIVEL DE LOS
        // JUGADORES EN EL MAPA.

        Id new_creature_id = this->next_instance_id;
        ++this->next_instance_id;

        this->creatures.emplace(
            std::piecewise_construct, std::forward_as_tuple(new_creature_id),
            std::forward_as_tuple(init_data, map_container, init_map,
                                  spawning_x_coord, spawning_y_coord,
                                  init_data.base_health,
                                  init_data.base_damage));

        fprintf(stderr, "NEW CREATURE: %s \n", init_data.name.c_str());

        _pushCreatureDifferentialBroadcast(new_creature_id, NEW_BROADCAST);
    }

    const Id Game::_randomSelectCreature() const {
        std::vector<Id> possible_creatures;

        const std::unordered_map<Id, CreatureCfg>& creatures_cfg =
            creatures_data.getMap();

        std::unordered_map<Id, CreatureCfg>::const_iterator it =
            creatures_cfg.begin();
        while (it != creatures_cfg.end()) {
            possible_creatures.push_back(it->first);
            ++it;
        }

        RandomNumberGenerator gen;
        return possible_creatures[gen(0, possible_creatures.size() - 1)];
    }

    void Game::_spawnNewCreature(const Id spawning_map) {
        const Id creature_id = _randomSelectCreature();

        const CreatureCfg& data = creatures_data[creature_id];

        this->newCreature(data, spawning_map);
    }

    void Game::spawnNewCreatures(const int it) {
        std::unordered_map<Id, MapCreaturesInfo>::iterator iterator =
            this->maps_creatures_info.begin();

        while (iterator != this->maps_creatures_info.end()) {
            if (iterator->second.amount_of_creatures == MAX_CREATURES_PER_MAP) {
                ++iterator;
                continue;
            }

            iterator->second.creature_spawning_cooldown -=
                it * TIME_TO_SPAWN_CREATURE;
            while (iterator->second.creature_spawning_cooldown <= 0) {
                _spawnNewCreature(iterator->first);
                ++iterator->second.amount_of_creatures;
                iterator->second.creature_spawning_cooldown +=
                    TIME_TO_SPAWN_CREATURE;
            }

            ++iterator;
        }
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
                Notification* reply =
                    new NotificationReply(ERROR_REPLY, e.what());
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

    void Game::actCreatures(const int it) {
        std::unordered_map<InstanceId, Creature>::iterator it_creatures =
            this->creatures.begin();

        while (it_creatures != this->creatures.end()) {
            it_creatures->second.act(it);

            if (it_creatures->second.mustBeBroadcasted()) {
                fprintf(stderr,
                        "\n\n ERROR: ACA NO DEBERIA ENTRAR TODAVIA \n\n");
                _pushCharacterDifferentialBroadcast(it_creatures->first,
                                                    UPDATE_BROADCAST, true);
            }

            // it_creatures->second.debug();

            ++it_creatures;
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
