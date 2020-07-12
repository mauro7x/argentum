#include <cstdio>  // debug
#include <list>
#include <string>
#include <utility>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/Exceptions/Exception.h"
#include "../../../Common/includes/Protocol.h"
#include "../../../Common/includes/RandomNumberGenerator.h"
//-----------------------------------------------------------------------------
#include "../../includes/Control/ActiveClients.h"
#include "../../includes/Control/Notifications/EntityBroadcast.h"
#include "../../includes/Control/Notifications/Event.h"
#include "../../includes/Control/Notifications/ItemBroadcast.h"
#include "../../includes/Control/Notifications/List.h"
#include "../../includes/Control/Notifications/Message.h"
#include "../../includes/Control/Notifications/Reply.h"
//-----------------------------------------------------------------------------
#include "../../includes/Model/Game.h"
//-----------------------------------------------------------------------------
#define FIRST_INSTANCE_ID 1
#define TIME_TO_NOTIFY_RESURRECT_COOLDOWN_INFO 5000
//-----------------------------------------------------------------------------

Game::Game(ActiveClients& active_clients, const int& rate, Database& database)
    : rate(rate),
      bank(items),
      next_instance_id(FIRST_INSTANCE_ID),
      active_clients(active_clients),
      database(database) {
    // Cargamos la configuración
    _loadCfg();
    data_persistence_cooldown = cfg.ms_to_persist_data;

    // Cargamos los mapas
    map_container.loadMaps();

    std::vector<Id> maps_id;
    this->map_container.getMapsId(maps_id);

    for (unsigned int i = 0; i < maps_id.size(); ++i) {
        this->maps_creatures_info.emplace(
            std::piecewise_construct, std::forward_as_tuple(maps_id[i]),
            std::forward_as_tuple(0, cfg.ms_to_spawn_creature));
    }

    std::vector<Id> npcs_id;
    npcs.gatherIds(npcs_id);
    for (size_t i = 0; i < npcs_id.size(); ++i) {
        if (npcs[npcs_id[i]].name == "Banquero")
            banker = npcs_id[i];
        else if (npcs[npcs_id[i]].name == "Sacerdote")
            priest = npcs_id[i];
        else
            merchants.push_back(npcs_id[i]);
    }

    _establishPriestsPosition(maps_id);
}

Game::~Game() {
    _persistAllData(this->database);
}

//-----------------------------------------------------------------------------
// Estructuras auxiliares
//-----------------------------------------------------------------------------

MapCreaturesInfo::MapCreaturesInfo(unsigned int amount_of_creatures,
                                   int creature_spawning_cooldown)
    : amount_of_creatures(amount_of_creatures),
      creature_spawning_cooldown(creature_spawning_cooldown) {}

//-----------------------------------------------------------------------------

ResurrectionInfo::ResurrectionInfo(int cooldown, int priest_x_coord,
                                   int priest_y_coord)
    : cooldown(cooldown),
      time_since_last_message(0),
      priest_x_coord(priest_x_coord),
      priest_y_coord(priest_y_coord) {}

//-----------------------------------------------------------------------------
// Métodos de carga de configuración
//-----------------------------------------------------------------------------

void Game::_loadCfg() {
    json config = JSON::loadJsonFile(paths::config(CONFIG_FILEPATH));
    cfg.critical_attack_dmg_modifier =
        config["game"]["critical_attack_dmg_modifier"];
    cfg.random_movement_factor = config["game"]["random_movement_factor"];
    cfg.max_creatures_per_map = config["game"]["max_creatures_per_map"];
    cfg.ms_to_update_character_attributes =
        config["game"]["ms_to_update_character_attributes"];
    cfg.ms_to_spawn_creature = config["game"]["ms_to_spawn_creature"];
    cfg.ms_to_disappear_dropped_item =
        config["game"]["ms_to_disappear_dropped_item"];
    cfg.ms_to_persist_data = config["game"]["ms_to_persist_data"];
}

//--------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Métodos auxiliares de creacion de entidades;
//-----------------------------------------------------------------------------

void Game::_establishPriestsPosition(std::vector<Id>& maps_id) {
    std::vector<Id>::iterator it = maps_id.begin();

    for (; it != maps_id.end(); ++it) {
        Map& map = map_container[*it];
        for (int x = 0; x < map.getWidthTiles(); ++x) {
            for (int y = 0; y < map.getHeightTiles(); ++y) {
                if (!(map.getTile(x, y).npc == priest))
                    continue;
                priests_position[*it].push_back(_coordinatesToMapKey(x, y));
            }
        }
    }
}

void Game::_loadBankAccount(const CharacterCfg& init_data) {
    BankAccount& account = bank[init_data.nickname];
    account.depositGold(init_data.bank_gold);
    for (size_t i = 0; i < init_data.bank_account.size(); i++) {
        const Id item = init_data.bank_account[i].item;
        const unsigned int amount = init_data.bank_account[i].amount;
        if (!item) {
            break;
        }
        account.deposit(item, amount);
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Métodos de broadcast
//-----------------------------------------------------------------------------

Notification* Game::_buildPlayerBroadcast(InstanceId id,
                                          BroadcastType broadcast_type) {
    PlayerData player_data;
    player_data.basic_data.gid = id;
    Character& character = this->characters.at(id);
    character.fillBroadcastData(player_data);

    Notification* broadcast =
        new EntityBroadcast(id, player_data, broadcast_type);

    return broadcast;
}

Notification* Game::_buildCreatureBroadcast(InstanceId id,
                                            BroadcastType broadcast_type) {
    CreatureData creature_data;
    creature_data.basic_data.gid = id;
    Creature& creature = this->creatures.at(id);
    creature.fillBroadcastData(creature_data);

    Notification* broadcast =
        new EntityBroadcast(id, creature_data, broadcast_type);

    return broadcast;
}

Notification* Game::_buildItemBroadcast(Id map_id, int x_coord, int y_coord,
                                        BroadcastType broadcast_type) {
    const Tile& tile = this->map_container[map_id].getTile(x_coord, y_coord);

    ItemData data;

    data.item_id = tile.item_id;
    data.x_tile = x_coord;
    data.y_tile = y_coord;
    data.amount = tile.item_amount;

    Notification* broadcast = new ItemBroadcast(data, map_id, broadcast_type);

    return broadcast;
}

void Game::_pushCharacterDifferentialBroadcast(InstanceId caller,
                                               BroadcastType broadcast_type,
                                               bool send_to_caller) {
    Notification* broadcast = _buildPlayerBroadcast(caller, broadcast_type);

    this->active_clients.sendDifferentialBroadcastToAll(broadcast, caller,
                                                        send_to_caller);

    if (this->characters.at(caller).getPosition().mustBeBroadcasted())
        _pushCharacterEvent(caller, MOVEMENT_EV_TYPE);

    this->characters.at(caller).beBroadcasted();
}

void Game::_pushCreatureDifferentialBroadcast(InstanceId creature,
                                              BroadcastType broadcast_type) {
    Notification* broadcast = _buildCreatureBroadcast(creature, broadcast_type);

    this->active_clients.sendDifferentialBroadcastToAll(broadcast, creature,
                                                        false);

    this->creatures.at(creature).beBroadcasted();
}

void Game::_pushItemDifferentialBroadcast(Id map_id, int x_coord, int y_coord,
                                          BroadcastType broadcast_type) {
    Notification* broadcast =
        _buildItemBroadcast(map_id, x_coord, y_coord, broadcast_type);

    this->active_clients.sendDifferentialBroadcastToAll(broadcast, 0, false);
}

void Game::_pushCharacterEvent(InstanceId id, EventType type) {
    const Position& position = this->characters.at(id).getPosition();
    Id map_id = position.getMapId();
    uint32_t x_coord = position.getX();
    uint32_t y_coord = position.getY();

    Notification* event = new Event(map_id, x_coord, y_coord, type);
    active_clients.sendEventToAll(event);
}

void Game::_pushCharacterMainEvent(InstanceId id, EventType type) {
    const Position& position = this->characters.at(id).getPosition();
    Id map_id = position.getMapId();
    uint32_t x_coord = position.getX();
    uint32_t y_coord = position.getY();

    Notification* event = new Event(map_id, x_coord, y_coord, type);
    active_clients.notify(id, event);
}

void Game::_pushCreatureEvent(InstanceId id, EventType type) {
    const Position& position = this->creatures.at(id).getPosition();
    Id map_id = position.getMapId();
    uint32_t x_coord = position.getX();
    uint32_t y_coord = position.getY();

    Notification* event = new Event(map_id, x_coord, y_coord, type);
    active_clients.sendEventToAll(event);
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
             * tiene que recibir sus propios datos, pues le llega un
             * DIFFERENTIAL UPDATE.
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

    Id receiver_map_id = this->characters.at(receiver).getMapId();
    std::unordered_map<std::string, int>::iterator it_items =
        dropped_items_lifetime_per_map[receiver_map_id].begin();

    while (it_items != dropped_items_lifetime_per_map[receiver_map_id].end()) {
        int x_coord, y_coord;
        _mapKeyToCoordinates(it_items->first, x_coord, y_coord);

        broadcast = _buildItemBroadcast(receiver_map_id, x_coord, y_coord,
                                        NEW_BROADCAST);

        ++it_items;
    }
}

void Game::broadcastNewCharacter(InstanceId id) {
    _pushFullBroadcast(id, true);
    _pushCharacterDifferentialBroadcast(id, NEW_BROADCAST, false);
}

//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Creación y eliminación de entidades
//----------------------------------------------------------------------------

const InstanceId Game::newCharacter(const CharacterCfg& init_data) {
    Id new_character_id = this->next_instance_id;
    ++this->next_instance_id;

    Id spawning_map_id = init_data.map;
    int spawning_x_coord, spawning_y_coord;

    if (init_data.new_created) {
        this->map_container[spawning_map_id].establishEntitySpawningPosition(
            spawning_x_coord, spawning_y_coord, false);
    } else {
        spawning_x_coord = init_data.x_tile;
        spawning_y_coord = init_data.y_tile;
        try {
            // Obtengo free tile próximo a la última posición del jugador
            // persistida.
            this->map_container[spawning_map_id].getNearestFreeTile(
                spawning_x_coord, spawning_y_coord, false);
        } catch (const CouldNotFindFreeTileException& e) {
            // Si no puede spawnear en la posición persistida, asignamos
            // posición aleatoria.
            this->map_container[spawning_map_id]
                .establishEntitySpawningPosition(spawning_x_coord,
                                                 spawning_y_coord, false);
        }
    }

    this->map_container[spawning_map_id].occupyTile(
        new_character_id, spawning_x_coord, spawning_y_coord);

    this->characters.emplace(
        std::piecewise_construct, std::forward_as_tuple(new_character_id),
        std::forward_as_tuple(init_data, this->races[init_data.race],
                              this->kinds[init_data.kind], this->map_container,
                              spawning_map_id, spawning_x_coord,
                              spawning_y_coord, this->items, formulas, rate,
                              cfg.critical_attack_dmg_modifier,
                              cfg.ms_to_update_character_attributes));

    this->nickname_id_map[init_data.nickname] = new_character_id;

    this->characters.at(new_character_id).debug();

    _loadBankAccount(init_data);

    return new_character_id;
}

void Game::newCreature(const CreatureCfg& init_data, const Id init_map) {
    Id new_creature_id = this->next_instance_id;
    ++this->next_instance_id;

    int spawning_x_coord;
    int spawning_y_coord;
    this->map_container[init_map].establishEntitySpawningPosition(
        spawning_x_coord, spawning_y_coord, true);

    this->map_container[init_map].occupyTile(new_creature_id, spawning_x_coord,
                                             spawning_y_coord);

    // AGREGAR LOGICA DE INIT_HEALTH E INIT_DAMAGE EN BASE AL NIVEL DE LOS
    // JUGADORES EN EL MAPA.

    this->creatures.emplace(
        std::piecewise_construct, std::forward_as_tuple(new_creature_id),
        std::forward_as_tuple(init_data, map_container, init_map,
                              spawning_x_coord, spawning_y_coord,
                              init_data.base_health, items, characters, *this,
                              formulas, rate, cfg.random_movement_factor));

    _pushCreatureDifferentialBroadcast(new_creature_id, NEW_BROADCAST);
}

void Game::deleteCharacter(const InstanceId id, Database& database) {
    if (!this->characters.count(id)) {
        throw Exception("deleteCharacter: Unknown character id [", id, "]");
    }
    Character& character = characters.at(id);
    _pushCharacterDifferentialBroadcast(id, DELETE_BROADCAST, false);

    CharacterCfg character_data = {};
    bank[character.getNickname()].fillPersistenceData(character_data);
    character.fillPersistenceData(character_data);
    database.persistPlayerData(character_data, true);

    this->nickname_id_map.erase(character.getNickname());

    this->bank.removeAccount(character.getNickname());

    this->characters.erase(id);
}

void Game::deleteCreature(const InstanceId id) {
    if (!this->creatures.count(id)) {
        throw Exception("deleteCreature: Unknown creature id [", id, "]");
    }

    Creature& creature = this->creatures.at(id);

    _pushCreatureDifferentialBroadcast(id, DELETE_BROADCAST);

    // Restamos en uno la cantidad de criaturas en ese mapa.
    --this->maps_creatures_info.at(creature.getMapId()).amount_of_creatures;

    this->creatures.erase(id);
}

const Id Game::_randomSelectCreature(const Id spawning_map) const {
    std::vector<Id> possible_creatures;

    possible_creatures = map_container[spawning_map].getCreatures();

    RandomNumberGenerator gen;
    return possible_creatures[gen(0, possible_creatures.size() - 1)];
}

void Game::_spawnNewCreature(const Id spawning_map) {
    const Id creature_id = _randomSelectCreature(spawning_map);

    const CreatureCfg& data = creatures_data[creature_id];

    this->newCreature(data, spawning_map);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Actualización del loop
//-----------------------------------------------------------------------------

void Game::actCharacters(const int it) {
    std::unordered_map<InstanceId, Character>::iterator it_characters =
        this->characters.begin();

    while (it_characters != this->characters.end()) {
        InstanceId id = it_characters->first;
        Character& character = it_characters->second;
        try {
            character.act(it);
        } catch (const CollisionWhileMovingException& e) {
            character.stopMoving();
            Notification* reply = new Reply(ERROR_MSG, e.what());
            active_clients.notify(id, reply);
        }

        if (character.mustBeBroadcasted())
            _pushCharacterDifferentialBroadcast(it_characters->first,
                                                UPDATE_BROADCAST, true);

        ++it_characters;
    }
}

void Game::actCreatures(const int it) {
    std::unordered_map<InstanceId, Creature>::iterator it_creatures =
        this->creatures.begin();

    while (it_creatures != this->creatures.end()) {
        InstanceId id = it_creatures->first;
        Creature& creature = it_creatures->second;
        creature.act(it);

        if (creature.mustBeBroadcasted()) {
            _pushCreatureDifferentialBroadcast(id, UPDATE_BROADCAST);
        }

        InstanceId receiver = creature.getAttackingCharacterId();

        if (receiver != 0) {
            Notification* broadcast =
                _buildPlayerBroadcast(receiver, UPDATE_BROADCAST);
            this->active_clients.notify(receiver, broadcast);
        }

        ++it_creatures;
    }
}

void Game::spawnNewCreatures(const int it) {
    std::unordered_map<Id, MapCreaturesInfo>::iterator iterator =
        this->maps_creatures_info.begin();

    while (iterator != this->maps_creatures_info.end()) {
        if (iterator->second.amount_of_creatures == cfg.max_creatures_per_map) {
            ++iterator;
            continue;
        }

        iterator->second.creature_spawning_cooldown -= it * rate;
        while (iterator->second.creature_spawning_cooldown <= 0) {
            _spawnNewCreature(iterator->first);
            ++iterator->second.amount_of_creatures;
            iterator->second.creature_spawning_cooldown +=
                cfg.ms_to_spawn_creature;
        }

        ++iterator;
    }
}

void Game::_persistAllData(Database& database) {
    std::unordered_map<InstanceId, Character>::iterator it_characters =
        this->characters.begin();

    while (it_characters != this->characters.end()) {
        CharacterCfg character_data = {};
        bank[it_characters->second.getNickname()].fillPersistenceData(
            character_data);
        it_characters->second.fillPersistenceData(character_data);
        database.persistPlayerData(character_data, false);
        ++it_characters;
    }
}

void Game::persistPeriodicData(Database& database, const int it) {
    data_persistence_cooldown -= it * rate;

    if (data_persistence_cooldown <= 0) {
        _persistAllData(database);
        data_persistence_cooldown = cfg.ms_to_persist_data;
    }
}

void Game::updateDroppedItemsLifetime(const int it) {
    // Recorremos primero los unordered_map de lifetime de items droppeados para
    // cada map_id. Luego, por cada item droppeado actualizamos su lifetime, y
    // si el mismo llega a cero (o menos) lo eliminamos del mapa.

    std::unordered_map<Id, std::unordered_map<std::string, int>>::iterator
        map_iterator = this->dropped_items_lifetime_per_map.begin();

    while (map_iterator != this->dropped_items_lifetime_per_map.end()) {
        std::unordered_map<std::string, int>::iterator dropped_items_iterator =
            map_iterator->second.begin();

        while (dropped_items_iterator != map_iterator->second.end()) {
            int& lifetime = dropped_items_iterator->second;

            lifetime -= it * rate;

            if (lifetime <= 0) {
                // Eliminamos el item del mapa.
                int x, y;
                _mapKeyToCoordinates(dropped_items_iterator->first, x, y);
                this->map_container[map_iterator->first].clearTileItem(x, y);
                _pushItemDifferentialBroadcast(map_iterator->first, x, y,
                                               DELETE_BROADCAST);
                dropped_items_iterator =
                    map_iterator->second.erase(dropped_items_iterator);
                continue;
            }

            ++dropped_items_iterator;
        }

        ++map_iterator;
    }
}

void Game::updateResurrectingPlayersCooldown(const int it) {
    std::unordered_map<Id, ResurrectionInfo>::iterator iterator =
        resurrecting_players_cooldown.begin();

    while (iterator != resurrecting_players_cooldown.end()) {
        Id player = iterator->first;

        if (!this->characters.count(player)) {
            iterator = resurrecting_players_cooldown.erase(iterator);
            continue;
        }

        ResurrectionInfo& info = iterator->second;

        info.cooldown -= it * rate;
        info.time_since_last_message += it * rate;

        if (info.cooldown <= 0) {
            // Resucitar.
            _cooldownResurrect(player);
            iterator = resurrecting_players_cooldown.erase(iterator);
            continue;
        }

        if (info.time_since_last_message >
                TIME_TO_NOTIFY_RESURRECT_COOLDOWN_INFO &&
            info.cooldown > 1000) {
            info.time_since_last_message = 0;
            std::string msg = "Te faltan " +
                              std::to_string(info.cooldown / 1000) +
                              " segundos para respawnear.";
            active_clients.notify(player, new Reply(INFO_MSG, msg));
        }

        ++iterator;
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Comandos
//-----------------------------------------------------------------------------

void Game::startMovingUp(const InstanceId caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::startMovingUp: unknown caller.");
    }

    Character& character = this->characters.at(caller);

    // StateCantMoveException se propaga.
    character.startMovingUp();
}

void Game::startMovingDown(const InstanceId caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::startMovingDown: unknown caller.");
    }

    Character& character = this->characters.at(caller);

    // StateCantMoveException se propaga.
    character.startMovingDown();
}

void Game::startMovingLeft(const InstanceId caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::startMovingLeft: unknown caller.");
    }

    Character& character = this->characters.at(caller);

    // StateCantMoveException se propaga.
    character.startMovingLeft();
}

void Game::startMovingRight(const InstanceId caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::startMovingRight: unknown caller.");
    }

    Character& character = this->characters.at(caller);

    // StateCantMoveException se propaga.
    character.startMovingRight();
}

void Game::stopMoving(const InstanceId caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::stopMoving: unknown caller.");
    }

    Character& character = this->characters.at(caller);
    character.stopMoving();
}

void Game::_dropAllItems(Attackable* dropper) {
    std::vector<DroppingSlot> dropped_items;
    dropper->dropAllItems(dropped_items);

    Id map_id = dropper->getMapId();
    int init_x = dropper->getPosition().getX();
    int init_y = dropper->getPosition().getY();

    for (unsigned int i = 0; i < dropped_items.size(); ++i) {
        int x = init_x;  // para no degenerar el dropeo en diagonal
        int y = init_y;  // para no degenerar el dropeo en diagonal

        try {
            this->map_container[map_id].addItem(dropped_items[i].item,
                                                dropped_items[i].amount, x, y);
        } catch (const CouldNotFindFreeTileException& e) {
            // No se pudo efectuar el dropeo. Cancelo.
            return;
        }

        // Agrego elemento al mapa de dropped items cooldown
        const std::string key = std::move(_coordinatesToMapKey(x, y));
        this->dropped_items_lifetime_per_map[map_id].emplace(
            key, cfg.ms_to_disappear_dropped_item);

        // Broadcasteo new item
        _pushItemDifferentialBroadcast(map_id, x, y, NEW_BROADCAST);
    }
}

void Game::_sendCharacterAttackNotifications(const int damage,
                                             const bool eluded,
                                             const InstanceId caller,
                                             const InstanceId target) {
    std::string msg_to_attacked, msg_to_attacker;

    WeaponType weapon_type = this->characters.at(caller).getWeaponType();

    if (damage > 0) {
        msg_to_attacker =
            "Le has infligido " + std::to_string(damage) + " de daño.";
        msg_to_attacked =
            "Has recibido " + std::to_string(damage) + " de daño.";
        if (weapon_type == EXPLOSIVE)
            _pushCharacterEvent(target, EXPLOSION_SPELL_EV_TYPE);
        else
            _pushCharacterEvent(caller, GRAL_ATTACK_EV_TYPE);

        _pushCharacterMainEvent(target, BEATTACKED_EV_TYPE);
        _pushCharacterEvent(target, DMG_EV_TYPE);

    } else if (damage <= 0 && weapon_type == HEALING) {
        msg_to_attacker =
            "Le has curado " + std::to_string(-damage) + " puntos de vida.";
        msg_to_attacked =
            "Te han curado " + std::to_string(-damage) + " puntos de vida.";

        _pushCharacterEvent(target, HEALING_SPELL_EV_TYPE);

    } else if (eluded) {
        msg_to_attacker = "Tu ataque fue eludido.";
        msg_to_attacked = "Has eludido un ataque.";
        _pushCharacterEvent(target, EVASION_EV_TYPE);
    } else {
        msg_to_attacker =
            "No le has causado daño, la defensa absorbió el ataque.";
        msg_to_attacked = "Tu defensa absorbió todo el daño del ataque.";

        // Provisorio, conviene ponerlo en un evento aparte.
        if (weapon_type == EXPLOSIVE)
            _pushCharacterEvent(target, EXPLOSION_SPELL_EV_TYPE);
        else
            _pushCharacterEvent(caller, GRAL_ATTACK_EV_TYPE);
    }

    Notification* reply = new Reply(INFO_MSG, msg_to_attacker.c_str());
    active_clients.notify(caller, reply);

    if (caller == target)
        return;

    reply = new Reply(INFO_MSG, msg_to_attacked.c_str());
    active_clients.notify(target, reply);
}

void Game::_sendCreatureAttackNotifications(const int damage,
                                            const InstanceId caller,
                                            const InstanceId target) {
    std::string msg_to_attacker = "Has atacado a la criatura, provocando " +
                                  std::to_string(damage) + " de daño.";

    WeaponType weapon_type = this->characters.at(caller).getWeaponType();

    if (weapon_type == EXPLOSIVE)
        _pushCreatureEvent(target, EXPLOSION_SPELL_EV_TYPE);
    else
        _pushCharacterEvent(caller, GRAL_ATTACK_EV_TYPE);

    _pushCreatureEvent(target, DMG_EV_TYPE);
    Notification* reply = new Reply(INFO_MSG, msg_to_attacker.c_str());
    active_clients.notify(caller, reply);
}

void Game::_sendAttackedByCreatureNotifications(const int damage,
                                                const bool eluded,
                                                const InstanceId target) {
    std::string msg_to_attacked;
    if (damage > 0) {
        msg_to_attacked =
            "Has recibido " + std::to_string(damage) + " de daño.";
        _pushCharacterMainEvent(target, BEATTACKED_EV_TYPE);
        _pushCharacterEvent(target, DMG_EV_TYPE);
    } else if (eluded) {
        msg_to_attacked = "Has eludido un ataque.";
        _pushCharacterEvent(target, EVASION_EV_TYPE);
    } else {
        msg_to_attacked = "Tu defensa absorbió todo el daño del ataque.";
    }
    Notification* reply = new Reply(INFO_MSG, msg_to_attacked.c_str());
    active_clients.notify(target, reply);
}

void Game::_useWeapon(const InstanceId caller, const InstanceId target,
                      Attackable* attacked, const bool target_is_creature) {
    Character& attacker = this->characters.at(caller);
    const unsigned int prev_level = attacker.getLevel();

    int damage = 0;
    bool eluded = false;

    /*
     * Se propagan excepciones:
     *  OutOfRangeAttackException,
     *  CantAttackWithoutWeaponException,
     *  KindCantDoMagicException,
     *  TooHighLevelDifferenceOnAttackException,
     *  NewbiesCantBeAttackedException, InsufficientManaException,
     *  AttackedActualStateCantBeAttackedException,
     *  AttackCooldownTimeNotElapsedException,
     *  CantAttackItselfException
     */
    eluded = attacker.useWeapon(attacked, damage);

    // Verificamos si murió, en cuyo caso dropea todo.
    if (target_is_creature)
        _sendCreatureAttackNotifications(damage, caller, target);
    else
        _sendCharacterAttackNotifications(damage, eluded, caller, target);
    if (damage > 0 && !attacked->getHealth()) {
        _dropAllItems(attacked);

        if (target_is_creature) {
            deleteCreature(target);
        } else {
            _pushCharacterEvent(target, DEATH_EV_TYPE);
        }
    }

    if (prev_level < attacker.getLevel())
        _pushCharacterEvent(caller, LEVEL_UP_EV_TYPE);
}

void Game::attackedByCreature(const InstanceId caller, int& damage,
                              bool eluded) {
    Character& attacked = this->characters.at(caller);
    if (damage > 0 && !attacked.getHealth()) {
        _pushCharacterEvent(caller, DEATH_EV_TYPE);
        _dropAllItems(&attacked);
    }
    _sendAttackedByCreatureNotifications(damage, eluded, caller);
}

void Game::useWeapon(const InstanceId caller, const InstanceId target) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::useWeapon: unknown caller.");
    }

    if (this->characters.count(target)) {
        Character& attacked = this->characters.at(target);
        _useWeapon(caller, target, &attacked, false);
    } else if (this->creatures.count(target)) {
        Creature& attacked = this->creatures.at(target);
        _useWeapon(caller, target, &attacked, true);
    } else {
        // Excepcion. target invalido.
        throw Exception("El target recibido no es válido.");
    }
}

void Game::equip(const InstanceId caller, const uint8_t n_slot) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::equip: unknown caller.");
    }

    Character& character = this->characters.at(caller);

    /* Se propagan excepciones:
     *   InvalidInventorySlotNumberException,
     *   KindCantDoMagicException,
     *   PotionHasNoPointsToRecoverException
     */
    character.equip(n_slot);
}

void Game::unequip(const InstanceId caller, const uint8_t n_slot) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::unequip: unknown caller.");
    }

    Character& character = this->characters.at(caller);

    /* Se propagan excepciones:
     *   FullInventoryException e
     *   InvalidEquipmentSlotNumberException
     */
    character.unequip(n_slot);
}

void Game::meditate(const InstanceId caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::unequip: unknown caller.");
    }

    Character& character = this->characters.at(caller);

    // Se propaga KindCantMeditateException
    character.meditate();

    std::string reply_msg =
        "Has comenzado a meditar. Ante cualquier acción dejarás de hacerlo.";

    Notification* reply = new Reply(SUCCESS_MSG, reply_msg);
    active_clients.notify(caller, reply);

    _pushCharacterEvent(caller, MEDITATE_EV_TYPE);
}

void Game::take(const InstanceId caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::equip: unknown caller.");
    }

    Character& character = this->characters.at(caller);
    Id map_id = character.getMapId();
    int x_coord = character.getPosition().getX();
    int y_coord = character.getPosition().getY();

    const Tile& tile = this->map_container[map_id].getTile(x_coord, y_coord);

    Id item_id = tile.item_id;
    uint32_t amount = tile.item_amount;

    if (!item_id)
        return;  // envio notifiacion?

    /*
     * Se propagan excepciones:
     *   FullInventoryException
     *   StateCantTakeItemException
     *   GoldMaximumCapacityReached
     */
    character.takeItem(this->items[item_id], amount);

    this->map_container[map_id].clearTileItem(x_coord, y_coord);

    _pushItemDifferentialBroadcast(map_id, x_coord, y_coord, DELETE_BROADCAST);

    _pushCharacterEvent(caller, GRAB_EV_TYPE);
}

const bool Game::_dropItem(const InstanceId caller, const uint8_t n_slot,
                           uint32_t& amount, Item** dropped) {
    Character& character = this->characters.at(caller);

    // Se propaga excepción InvalidInventorySlotNumberException
    *dropped = character.dropItem(n_slot, amount);

    return (*dropped != nullptr);
}

void Game::drop(const InstanceId caller, const uint8_t n_slot,
                uint32_t amount) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::equip: unknown caller.");
    }

    uint32_t asked_amount = amount;
    Item* dropped = nullptr;

    // Se propaga excepción InvalidInventorySlotNumberException
    if (!_dropItem(caller, n_slot, amount, &dropped))
        return;

    if (amount < asked_amount) {
        std::string reply_msg =
            "Se dropearon únicamente " + std::to_string(amount) + " items.";
        Notification* reply = new Reply(INFO_MSG, reply_msg.c_str());
        active_clients.notify(caller, reply);
    }

    Character& character = this->characters.at(caller);

    Id dropped_item_id = dropped->getId();
    int x = character.getPosition().getX();
    int y = character.getPosition().getY();
    Id map_id = character.getMapId();

    try {
        this->map_container[map_id].addItem(dropped_item_id, amount, x, y);
    } catch (const CouldNotFindFreeTileException& e) {
        // No se pudo efectuar el dropeo. Le devuelvo el item al character.
        character.takeItem(dropped, amount);

        // Propago la excepción.
        throw e;
    }

    // Agrego elemento al mapa de dropped items cooldown
    const std::string key = std::move(_coordinatesToMapKey(x, y));
    this->dropped_items_lifetime_per_map[map_id].emplace(
        key, cfg.ms_to_disappear_dropped_item);

    // Broadcasteo new item
    _pushItemDifferentialBroadcast(map_id, x, y, NEW_BROADCAST);

    Notification* event = new Event(map_id, x, y, THROW_EV_TYPE);
    active_clients.sendEventToAll(event);
}

void Game::_cooldownResurrect(const InstanceId caller) {
    if (!this->characters.count(caller))
        return;  // El jugador se desconectó. Seguirá muerto.

    Character& character = this->characters.at(caller);
    Id map_id = character.getMapId();
    ResurrectionInfo& info = resurrecting_players_cooldown.at(caller);

    character.resurrect();

    int respawn_x = info.priest_x_coord;
    int respawn_y = info.priest_y_coord;

    try {
        this->map_container[map_id].getNearestFreeTile(respawn_x, respawn_y,
                                                       false);
    } catch (const CouldNotFindFreeTileException& e) {
        // Que hago aca? je.
        return;
    }

    this->map_container[map_id].clearTileOccupant(
        character.getPosition().getX(), character.getPosition().getY());
    this->map_container[map_id].occupyTile(caller, respawn_x, respawn_y);
    character.teleport(map_id, respawn_x, respawn_y);

    _pushCharacterDifferentialBroadcast(caller, UPDATE_BROADCAST, true);
    _notifyResurrection(caller);
}

void Game::resurrect(const InstanceId caller) {
    if (!this->characters.count(caller))
        throw Exception("Game::resurrect: unknown caller.");

    Character& character = this->characters.at(caller);

    if (this->resurrecting_players_cooldown.count(caller))
        throw Exception(
            "Ya estás resucitando. Sé paciente. Todavía debes esperar %i "
            "segundos.",
            this->resurrecting_players_cooldown.at(caller).cooldown / 1000);

    // Propago excepción StateCantResurrectException
    character.startResurrecting();

    // encontrar al sacerdote más cercano.
    std::vector<std::string>& priests =
        this->priests_position[character.getMapId()];

    if (!priests.size())
        throw Exception("No hay sacerdotes en el mapa.");

    std::vector<std::string>::iterator it = priests.begin();
    int respawn_x_coord, respawn_y_coord;
    _mapKeyToCoordinates(*it, respawn_x_coord, respawn_y_coord);
    unsigned int min_distance =
        character.getPosition().getDistance(respawn_x_coord, respawn_y_coord);

    for (++it; it != priests.end(); ++it) {
        int _x, _y;
        _mapKeyToCoordinates(*it, _x, _y);
        unsigned int distance = character.getPosition().getDistance(_x, _y);
        if (distance < min_distance) {
            respawn_x_coord = _x;
            respawn_y_coord = _y;
            min_distance = distance;
        }
    }

    // calcular cooldown.
    unsigned int cooldown = min_distance * 1000;

    this->resurrecting_players_cooldown.emplace(
        std::piecewise_construct, std::forward_as_tuple(caller),
        std::forward_as_tuple(cooldown, respawn_x_coord, respawn_y_coord));

    std::string reply_msg = "Resucitando. Debes esperar " +
                            std::to_string(cooldown / 1000) +
                            " segundos de cooldown.";
    Notification* reply = new Reply(SUCCESS_MSG, reply_msg);
    active_clients.notify(caller, reply);
}

void Game::_notifyResurrection(const InstanceId caller) {
    std::string reply_msg = "¡Has resucitado!";
    Notification* reply = new Reply(SUCCESS_MSG, reply_msg);
    active_clients.notify(caller, reply);

    _pushCharacterEvent(caller, RESURRECT_EV_TYPE);
}

void Game::resurrect(const InstanceId caller, const uint32_t x_coord,
                     const uint32_t y_coord) {
    if (!this->characters.count(caller))
        throw Exception("Game::resurrect: unknown caller.");

    if (this->resurrecting_players_cooldown.count(caller))
        throw Exception(
            "Ya estás resucitando. Sé paciente. Todavía debes esperar %i.",
            this->resurrecting_players_cooldown.at(caller).cooldown);

    Id priest_id;

    // Propaga Exception si la posición especificada no corresponde a la de un
    // sacerdote
    _validatePriestPosition(caller, priest_id, x_coord, y_coord, true);

    Character& character = this->characters.at(caller);

    // Propaga StateCantResurrectException
    character.resurrect();

    _notifyResurrection(caller);
}

void Game::heal(const InstanceId caller, const uint32_t x_coord,
                const uint32_t y_coord) {
    if (!this->characters.count(caller))
        throw Exception("Game::heal: unknown caller.");

    Id priest_id;

    // Propaga Exception si la posición especificada no corresponde a la de un
    // sacerdote
    _validatePriestPosition(caller, priest_id, x_coord, y_coord, true);

    Character& character = this->characters.at(caller);

    // Propaga StateCantBeHealedException.
    character.heal();

    std::string reply_msg = "Has sido curado.";
    Notification* reply = new Reply(SUCCESS_MSG, reply_msg);
    active_clients.notify(caller, reply);

    _pushCharacterEvent(caller, HEALED_BY_PRIEST_EV_TYPE);
}

void Game::list(const InstanceId caller, const uint32_t x_coord,
                const uint32_t y_coord) {
    if (!this->characters.count(caller))
        throw Exception("Game::list: unknown caller.");

    Character& character = this->characters.at(caller);

    Id npc_id;

    std::string init_msg;
    std::list<std::string> list_items;

    if (_validateBankerPosition(caller, npc_id, x_coord, y_coord, false)) {
        BankAccount& account = bank[character.getNickname()];
        account.list(init_msg, list_items);
    } else if (_validatePriestPosition(caller, npc_id, x_coord, y_coord,
                                       false) ||
               _validateMerchantPosition(caller, npc_id, x_coord, y_coord,
                                         false)) {
        _listNPCSellableItems(npc_id, init_msg, list_items);
    } else if (_validatePortalPosition(caller, x_coord, y_coord, false)) {
        _listPortalMaps(init_msg, list_items);
    } else {
        throw Exception(
            "La posición indicada no corresponde a la de un NPC listable.");
    }

    Notification* list = new List(init_msg, list_items);
    this->active_clients.notify(caller, list);
    return;
}

void Game::_listPortalMaps(std::string& init_msg,
                           std::list<std::string>& item_list) {
    init_msg = "Posibles mapas para viajar";

    std::vector<Id> maps_id;
    this->map_container.getMapsId(maps_id);

    std::vector<Id>::iterator it = maps_id.begin();
    for (; it != maps_id.end(); ++it) {
        item_list.push_back(std::to_string(*it) + ": " +
                            this->map_container[*it].getMapName());
    }
}

void Game::_listNPCSellableItems(const Id npc_id, std::string& init_msg,
                                 std::list<std::string>& item_list) {
    init_msg = "Items en venta";

    const std::list<Id>& sellable_items = npcs[npc_id].sellable_items;
    std::list<Id>::const_iterator it = sellable_items.begin();

    std::string item;
    for (; it != sellable_items.end(); ++it) {
        item = std::to_string(*it) + ": " + items[*it]->what() + " ($" +
               std::to_string(items[*it]->getPrice()) + ") ";
        item_list.push_back(item);
    }
}

const bool Game::_validateBankerPosition(const InstanceId caller, Id& npc_id,
                                         const uint32_t x_coord,
                                         const uint32_t y_coord,
                                         const bool exception_if_invalid) {
    Id map_id = this->characters.at(caller).getMapId();

    if (this->map_container[map_id].getTile(x_coord, y_coord).npc == banker) {
        npc_id = banker;
        return true;
    }

    if (exception_if_invalid)
        throw Exception("El NPC seleccionado no es un banquero.");

    return false;
}

const bool Game::_validatePriestPosition(const InstanceId caller, Id& npc_id,
                                         const uint32_t x_coord,
                                         const uint32_t y_coord,
                                         const bool exception_if_invalid) {
    Id map_id = this->characters.at(caller).getMapId();

    if (this->map_container[map_id].getTile(x_coord, y_coord).npc == priest) {
        npc_id = priest;
        return true;
    }

    if (exception_if_invalid)
        throw Exception("El NPC seleccionado no es un sacerdote.");

    return false;
}

const bool Game::_validateMerchantPosition(const InstanceId caller, Id& npc_id,
                                           const uint32_t x_coord,
                                           const uint32_t y_coord,
                                           const bool exception_if_invalid) {
    Id map_id = this->characters.at(caller).getMapId();
    Id npc_tile_id = this->map_container[map_id].getTile(x_coord, y_coord).npc;

    for (size_t i = 0; i < merchants.size(); ++i) {
        if (merchants[i] != npc_tile_id)
            continue;

        npc_id = npc_tile_id;
        return true;
    }

    if (exception_if_invalid)
        throw Exception("El NPC seleccionado no es un mercader.");

    return false;
}

void Game::depositItemOnBank(const InstanceId caller, const uint32_t x_coord,
                             const uint32_t y_coord, const uint8_t n_slot,
                             uint32_t amount) {
    if (!this->characters.count(caller))
        throw Exception("Game::depositItemOnBank: unknown caller.");

    Id banker_id;

    // Propaga Exception si la posición especificada no corresponde a la de un
    // banquero
    _validateBankerPosition(caller, banker_id, x_coord, y_coord, true);

    uint32_t asked_amount = amount;
    Item* to_deposit = nullptr;

    // Se propaga excepción InvalidInventorySlotNumberException
    if (!_dropItem(caller, n_slot, amount, &to_deposit))
        return;

    Character& character = this->characters.at(caller);

    BankAccount& account = this->bank[character.getNickname()];

    // Propaga FullBankAccountException.
    account.deposit(to_deposit->getId(), amount);

    std::string reply_msg = "";

    if (amount < asked_amount)
        reply_msg += "No tenés suficientes items.";

    reply_msg += "Se ha depositado " + to_deposit->what() + " x" +
                 std::to_string(amount) + " en el banco";

    Notification* reply = new Reply(INFO_MSG, reply_msg.c_str());
    active_clients.notify(caller, reply);
}

void Game::withdrawItemFromBank(const InstanceId caller, const uint32_t x_coord,
                                const uint32_t y_coord, const uint32_t item_id,
                                uint32_t amount) {
    if (!this->characters.count(caller))
        throw Exception("Game::withdrawItemFromBank: unknown caller.");

    Id banker_id;

    // Propaga Exception si la posición especificada no corresponde a la de un
    // banquero
    _validateBankerPosition(caller, banker_id, x_coord, y_coord, true);

    uint32_t asked_amount = amount;
    Item* withdrew = nullptr;

    Character& character = this->characters.at(caller);

    BankAccount& account = this->bank[character.getNickname()];

    // Propaga InvalidItemIdException.
    withdrew = account.withdraw(item_id, amount);

    try {
        character.takeItem(withdrew, amount);
    } catch (const std::exception& e) {
        // StateCantTakeItemsException, FullInventoryException
        account.deposit(withdrew->getId(), amount);
        throw Exception("%s", e.what());  // Se propaga.
    }

    std::string reply_msg = "";

    if (asked_amount > amount)
        reply_msg += "No tenías tantos items. ";

    reply_msg += "Se ha extraído " + withdrew->what() + " x" +
                 std::to_string(amount) + " del banco";

    Notification* reply = new Reply(INFO_MSG, reply_msg.c_str());
    active_clients.notify(caller, reply);
}

void Game::depositGoldOnBank(const InstanceId caller, const uint32_t x_coord,
                             const uint32_t y_coord, const uint32_t amount) {
    if (!this->characters.count(caller))
        throw Exception("Game::depositGoldOnBank: unknown caller.");

    Id banker_id;

    // Propaga Exception si la posición especificada no corresponde a la de un
    // banquero
    _validateBankerPosition(caller, banker_id, x_coord, y_coord, true);

    Character& character = this->characters.at(caller);

    // Propaga InsufficientGoldException, StateCantGatherGoldException.
    character.gatherGold(amount);

    BankAccount& account = this->bank[character.getNickname()];

    account.depositGold(amount);

    std::string reply_msg =
        "Se han depositado " + std::to_string(amount) + " de oro en el banco";

    Notification* reply = new Reply(INFO_MSG, reply_msg.c_str());
    active_clients.notify(caller, reply);
}

void Game::withdrawGoldFromBank(const InstanceId caller, const uint32_t x_coord,
                                const uint32_t y_coord, uint32_t amount) {
    if (!this->characters.count(caller))
        throw Exception("Game::withdrawGoldFromBank: unknown caller.");

    Id banker_id;

    // Propaga Exception si la posición especificada no corresponde a la de un
    // banquero
    _validateBankerPosition(caller, banker_id, x_coord, y_coord, true);

    uint32_t asked_amount = amount;

    Character& character = this->characters.at(caller);

    BankAccount& account = this->bank[character.getNickname()];

    // Propaga NoMoneyAvailableException
    account.withdrawGold(amount);

    std::string reply_msg = "";

    try {
        character.takeGold(amount);
    } catch (const GoldMaximumCapacityReachedException& e) {
        account.depositGold(asked_amount - amount);
        reply_msg +=
            "No pudiste extraer todo lo solicitado. No entra más oro en tu "
            "inventario.";
    } catch (const StateCantTakeItemException& e) {
        account.depositGold(amount);
        throw e;
    }

    // agregar mensaje si pide extraer mas de lo que tiene

    reply_msg += "Se ha extraído " + std::to_string(amount) + " oro del banco.";

    Notification* reply = new Reply(INFO_MSG, reply_msg.c_str());
    active_clients.notify(caller, reply);
}

void Game::_validateIfNPCSellsItem(const InstanceId caller, const Id npc_id,
                                   const Id item_id) {
    const std::list<Id>& sellable_items = npcs[npc_id].sellable_items;
    std::list<Id>::const_iterator it =
        std::find(sellable_items.begin(), sellable_items.end(), item_id);

    if (it != sellable_items.end())
        return;

    throw Exception(
        "El item especificado no es válido. [Escriba /listar para ver los "
        "items válidos].");
}

void Game::buyItem(const InstanceId caller, const uint32_t x_coord,
                   const uint32_t y_coord, const uint32_t item_id,
                   const uint32_t amount) {
    if (!this->characters.count(caller))
        throw Exception("Game::buyItem: unknown caller.");

    fprintf(stderr, "buyItem: item_id: %i, amount: %i \n", item_id, amount);

    Character& character = this->characters.at(caller);

    // Verifico x e y correspondan a la posición de un comerciante o sacerdote.
    Id npc_id = 0;

    if (!_validatePriestPosition(caller, npc_id, x_coord, y_coord, false) &&
        !_validateMerchantPosition(caller, npc_id, x_coord, y_coord, false))
        throw Exception("No seleccionaste a un NPC que venda items.");

    // Propaga Exception si no vende el item especificado.
    _validateIfNPCSellsItem(caller, npc_id, item_id);

    unsigned int total_price = items[item_id]->getPrice() * amount;

    // Propaga InsufficientGoldException, StateCantGatherGoldException
    character.gatherGold(total_price);

    try {
        character.takeItem(this->items[item_id], amount);
    } catch (const FullInventoryException& e) {
        // Le devuelvo el oro.
        character.takeGold(total_price);
        throw e;
    }

    std::string reply_msg =
        "Has comprado " + std::to_string(amount) + " " + items[item_id]->what();
    Notification* reply = new Reply(SUCCESS_MSG, reply_msg);
    active_clients.notify(caller, reply);
}

void Game::sellItem(const InstanceId caller, const uint32_t x_coord,
                    const uint32_t y_coord, const uint8_t n_slot,
                    uint32_t amount) {
    if (!this->characters.count(caller))
        throw Exception("Game::buyItem: unknown caller.");

    Character& character = this->characters.at(caller);

    // Verifico x e y correspondan a la posición de un comerciante o sacerdote.
    Id npc_id = 0;

    if (!_validatePriestPosition(caller, npc_id, x_coord, y_coord, false) &&
        !_validateMerchantPosition(caller, npc_id, x_coord, y_coord, false))
        throw Exception("No seleccionaste a un NPC que compre items.");

    // uint32_t asked_amount = amount;
    Item* to_sell = nullptr;

    // Propago InvalidInventorySlotNumberException
    if (!_dropItem(caller, n_slot, amount, &to_sell))
        return;

    try {
        _validateIfNPCSellsItem(caller, npc_id, to_sell->getId());
    } catch (const Exception& e) {
        character.takeItem(to_sell, amount);
        throw e;
    }

    unsigned int total_price = to_sell->getPrice() * amount;
    unsigned int taken_amount = total_price;

    try {
        character.takeGold(taken_amount);
    } catch (const GoldMaximumCapacityReachedException& e) {
        // GoldMaximumCapacityReachedException
        unsigned int amount_to_replenish =
            (total_price - taken_amount) / to_sell->getPrice();
        fprintf(stderr, "Amount to replenish: %i\n", amount_to_replenish);
        if (amount_to_replenish)
            character.takeItem(to_sell, amount_to_replenish);
        throw e;
    }

    // si amount < asked_amount avisar.

    std::string reply_msg =
        "Has vendido " + std::to_string(amount) + " " + to_sell->what();
    Notification* reply = new Reply(SUCCESS_MSG, reply_msg);
    active_clients.notify(caller, reply);
}

void Game::sendPrivateMessage(const InstanceId caller,
                              const std::string to_nickname,
                              const std::string message) {
    if (!this->characters.count(caller))
        throw Exception("Game::sendPrivateMessage: unknown caller.");

    if (!this->nickname_id_map.count(to_nickname))
        throw Exception("No existe jugador con el nickname especificado.");

    InstanceId destinatary_id = this->nickname_id_map.at(to_nickname);

    const std::string& caller_nickname =
        this->characters.at(caller).getNickname();

    Notification* receiver_notification =
        new Message(caller_nickname, message, PRIVATE_MSG);
    this->active_clients.notify(destinatary_id, receiver_notification);

    std::string caller_msg = "A " + to_nickname;

    Notification* sender_notification =
        new Message(caller_msg, message, PRIVATE_MSG);
    this->active_clients.notify(caller, sender_notification);
}

void Game::sendGeneralMessage(const InstanceId caller,
                              const std::string message) {
    if (!this->characters.count(caller))
        throw Exception("Game::sendPrivateMessage: unknown caller.");

    const std::string& caller_nickname =
        this->characters.at(caller).getNickname();

    Notification* notification =
        new Message(caller_nickname, message, GENERAL_MSG);
    this->active_clients.sendMessageToAll(notification);
}

void Game::listConnectedPlayers(const InstanceId caller) {
    if (!this->characters.count(caller))
        throw Exception("Game::listConnectedPlayers: unknown caller.");

    std::unordered_map<Id, Character>::const_iterator it =
        this->characters.begin();

    std::string init_msg = "Jugadores conectados";
    std::list<std::string> player_list;

    for (; it != this->characters.end(); ++it) {
        player_list.push_back(it->second.getNickname());
    }

    Notification* list = new List(init_msg, player_list);
    active_clients.notify(caller, list);
}

void Game::help(const InstanceId caller, const uint32_t x_coord,
                const uint32_t y_coord) {
    if (!this->characters.count(caller))
        throw Exception("Game::helpNPC: unknown caller.");

    std::string init_msg;
    std::list<std::string> descriptions;

    Id npc_id = 0;

    if (_validatePriestPosition(caller, npc_id, x_coord, y_coord, false)) {
        init_msg = "Sacerdote";
        descriptions.push_back("/resucitar: resucita a un jugador muerto");
        descriptions.push_back(
            "/curar: cura a un personaje tanto en maná como en vida.");
        descriptions.push_back(
            "/comprar <objeto> [<cantidad>]: para comprar los báculos, varas y "
            "pociones en su lista.");
        descriptions.push_back(
            "/vender [<cantidad>]: para vender el item seleccionado del "
            "inventario al precio de su lista.");
        descriptions.push_back(
            "/listar: para conocer los items que compra/vende.");
    } else if (_validateBankerPosition(caller, npc_id, x_coord, y_coord,
                                       false)) {
        init_msg = "Banquero";
        descriptions.push_back(
            "/depositar [<cantidad>]: para guardar el item seleccionado del "
            "inventario en la cuenta bancaria.");
        descriptions.push_back(
            "/depositaroro <cantidad>: para depositar el monto de oro "
            "especificado en la cuenta bancaria.");
        descriptions.push_back(
            "/retirar <id> [<cantidad>]: para retirar el item especificado de "
            "la cuenta bancaria.");
        descriptions.push_back(
            "/retiraroro <cantidad>: para retirar el monto especificado de oro "
            "de la cuenta bancaria.");
        descriptions.push_back(
            "/listar: para listar el contenido de la cuenta bancaria.");
    } else if (_validateMerchantPosition(caller, npc_id, x_coord, y_coord,
                                         false)) {
        init_msg = "Mercader";
        descriptions.push_back(
            "/comprar <objeto> [<cantidad>]: para comprar las armas y defensas "
            "en su lista.");
        descriptions.push_back(
            "/vender [<cantidad>]: para vender el item seleccionado del "
            "inventario al precio de su lista.");
        descriptions.push_back(
            "/listar: para conocer los items que compra/vende.");
    } else {
        // En la posición recibida no hay un npc.
        throw Exception("Hay que seleccionar a un NPC para la ayuda.");
    }

    Notification* list = new List(init_msg, descriptions);
    active_clients.notify(caller, list);
}

const bool Game::_validatePortalPosition(const InstanceId caller,
                                         const uint32_t x_coord,
                                         const uint32_t y_coord,
                                         const bool exception_if_invalid) {
    Id map_id = this->characters.at(caller).getMapId();

    if (this->map_container[map_id].getTile(x_coord, y_coord).portal)
        return true;

    if (exception_if_invalid)
        throw Exception("No hay un portal en la posición especificada.");

    return false;
}

void Game::_validatePortalMapId(const InstanceId caller, Id map_id) {
    std::vector<Id> maps_id;
    this->map_container.getMapsId(maps_id);

    std::vector<Id>::iterator it =
        std::find(maps_id.begin(), maps_id.end(), map_id);

    if (it == maps_id.end())
        throw Exception("El Id del mapa es inválido.");
}

void Game::teleport(const InstanceId caller, const uint32_t portal_x_coord,
                    const uint32_t portal_y_coord, const Id map_id) {
    if (!this->characters.count(caller))
        throw Exception("Game::teleport: unknown caller.");

    Character& character = this->characters.at(caller);

    // Si está resucitando, no se puede teletransportar.
    if (this->resurrecting_players_cooldown.count(caller))
        throw Exception(
            "No puedes teletransportarte mientras estás resucitando.");

    // Propaga Exception si no hay un portal en la posición especificada.
    _validatePortalPosition(caller, portal_x_coord, portal_y_coord, true);

    if (map_id == character.getMapId())
        throw Exception(
            "No puedes viajar al mismo mapa en el que te encuentras.");

    _validatePortalMapId(caller, map_id);

    _pushCharacterDifferentialBroadcast(caller, DELETE_BROADCAST, false);

    int spawning_x_coord, spawning_y_coord;
    this->map_container[map_id].establishEntitySpawningPosition(
        spawning_x_coord, spawning_y_coord, false);
    this->map_container[map_id].occupyTile(caller, spawning_x_coord,
                                           spawning_y_coord);

    character.teleport(map_id, spawning_x_coord, spawning_y_coord);
    active_clients.changeMap(caller, map_id);

    _pushCharacterDifferentialBroadcast(caller, NEW_BROADCAST, false);

    // este podria ser reemplazado por uno solo hacia el caller
    _pushCharacterDifferentialBroadcast(caller, UPDATE_BROADCAST, true);

    _pushFullBroadcast(caller, false);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Getters de atributos
//-----------------------------------------------------------------------------

const Id Game::getMapId(const InstanceId caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game.cpp::getMapId: unknown caller.");
    }

    return this->characters.at(caller).getMapId();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Funciones auxiliares
//-----------------------------------------------------------------------------

const std::string _coordinatesToMapKey(int x, int y) {
    return std::move(std::to_string(x) + "," + std::to_string(y));
}

void _mapKeyToCoordinates(const std::string& key, int& x, int& y) {
    std::size_t delim = key.find(',');
    x = std::stoi(key.substr(0, delim));
    y = std::stoi(key.substr(delim + 1));
}

//-----------------------------------------------------------------------------
