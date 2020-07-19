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
#define INVALID_PORTAL_COORDINATES_ERROR_MSG \
    "No hay un portal en la posición especificada."
#define INVALID_PRIEST_COORDINATES_ERROR_MSG \
    "No hay un sacerdote en la posición especificada."
#define INVALID_MERCHANT_COORDINATES_ERROR_MSG \
    "No hay un mercader en la posición especificada."
#define INVALID_BANKER_COORDINATES_ERROR_MSG \
    "No hay un banquero en la posición especificada."
#define INVALID_SELLER_COORDINATES_ERROR_MSG \
    "No seleccionaste a un NPC que venda items."
#define INVALID_MARKETING_ITEM_ERROR_MSG                                       \
    "El item especificado no es válido. [Escriba /listar para ver los items " \
    "válidos]."
#define INVALID_NICKNAME_ERROR_MSG \
    "No existe jugador con el nickname especificado."
#define TELEPORT_WHILE_RESURRECTING_ERROR_MSG \
    "No puedes teletransportarte mientras estás resucitando."
#define TELEPORT_TO_ACTUAL_MAP_ERROR_MSG \
    "No puedes viajar al mismo mapa en el que te encuentras."
#define INVALID_PORTAL_MAP_ID_ERROR_MSG "El map id especificado es inválido."
#define INVALID_ATTACK_TARGET_ERROR_MSG "El target recibido no es válido."
#define INSUFFICIENT_AMOUNT_OF_ITEMS_ERROR_MSG \
    "No tenés la cantidad solicitada de items."
#define BANK_ACCOUNT_INVALID_ITEM_ERROR_MSG \
    "No tienes ningún item con ese id en tu cuenta."
#define NO_LISTABLE_NPC_ERROR_MSG \
    "La posición indicada no corresponde a la de un NPC listable."
#define NO_HELP_NPC_SELECTED_ERROR_MSG \
    "Hay que seleccionar a un NPC para la ayuda."
//-----------------------------------------------------------------------------
#define RESURRECTED_SUCCESS_MSG "¡Has resucitado!"
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
    logic_maps.loadMedia();

    std::vector<Id> maps_id;
    logic_maps.getMapsId(maps_id);

    // Por cada mapa, agregamos MapCreaturesInfo
    for (unsigned int i = 0; i < maps_id.size(); ++i) {
        this->maps_creatures_info.emplace(
            std::piecewise_construct, std::forward_as_tuple(maps_id[i]),
            std::forward_as_tuple(0, cfg.ms_to_spawn_creature));
    }

    // Configuramos los id de los distintos NPCs
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

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Métodos auxiliares de creacion de game
//-----------------------------------------------------------------------------

void Game::_establishPriestsPosition(std::vector<Id>& maps_id) {
    std::vector<Id>::iterator it = maps_id.begin();

    for (; it != maps_id.end(); ++it) {
        Map& map = logic_maps.getMap(*it);
        for (int x = 0; x < map.getWidthTiles(); ++x) {
            for (int y = 0; y < map.getHeightTiles(); ++y) {
                if (!(map.getTile(x, y).npc == priest))
                    continue;
                priests_position[*it].push_back(_coordinatesToMapKey(x, y));
            }
        }
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Métodos auxiliares de creacion de characters
//-----------------------------------------------------------------------------

void Game::_establishCharacterSpawningPosition(const CharacterCfg& init_data,
                                               const Id& spawning_map_id,
                                               int& spawning_x_coord,
                                               int& spawning_y_coord) {
    if (init_data.new_created) {
        this->logic_maps.establishEntitySpawningPosition(
            spawning_map_id, spawning_x_coord, spawning_y_coord, false);
    } else {
        spawning_x_coord = init_data.x_tile;
        spawning_y_coord = init_data.y_tile;
        try {
            // Obtengo free tile próximo a la última posición del jugador
            // persistida.
            this->logic_maps.getNearestFreeTile(
                spawning_map_id, spawning_x_coord, spawning_y_coord, false);
        } catch (const CouldNotFindFreeTileException& e) {
            // Si no puede spawnear en la posición persistida, asignamos
            // posición aleatoria.
            this->logic_maps.establishEntitySpawningPosition(
                spawning_map_id, spawning_x_coord, spawning_y_coord, false);
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
// Métodos auxiliares de creacion de creatures
//-----------------------------------------------------------------------------

const Id Game::_randomSelectCreature(const Id spawning_map) const {
    const std::vector<Id>& possible_creatures =
        logic_maps.getCreatures(spawning_map);

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
    const Tile& tile = this->logic_maps.getTile(map_id, x_coord, y_coord);
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

    // Broadcasteamos todos los characters, excepto el del caller
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

    // Broadcasteamos todas las criaturas
    std::unordered_map<InstanceId, Creature>::iterator it_creatures =
        this->creatures.begin();

    while (it_creatures != this->creatures.end()) {
        broadcast = _buildCreatureBroadcast(it_creatures->first, NEW_BROADCAST);

        this->active_clients.notify(receiver, broadcast);

        ++it_creatures;
    }

    // Broadcasteamos todos los items dropeados
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

//-----------------------------------------------------------------------------
// Métodos auxiliares de persistencia
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Validación de presencia de entes según la posición
//-----------------------------------------------------------------------------

const bool Game::_validateBankerPosition(const InstanceId caller, Id& npc_id,
                                         const uint32_t x_coord,
                                         const uint32_t y_coord) {
    Id map_id = this->characters.at(caller).getMapId();
    if (this->logic_maps.getTile(map_id, x_coord, y_coord).npc == banker) {
        npc_id = banker;
        return true;
    }

    return false;
}

const bool Game::_validatePriestPosition(const InstanceId caller, Id& npc_id,
                                         const uint32_t x_coord,
                                         const uint32_t y_coord) {
    Id map_id = this->characters.at(caller).getMapId();
    if (this->logic_maps.getTile(map_id, x_coord, y_coord).npc == priest) {
        npc_id = priest;
        return true;
    }

    return false;
}

const bool Game::_validateMerchantPosition(const InstanceId caller, Id& npc_id,
                                           const uint32_t x_coord,
                                           const uint32_t y_coord) {
    Id map_id = this->characters.at(caller).getMapId();
    Id npc_tile_id = this->logic_maps.getTile(map_id, x_coord, y_coord).npc;
    for (size_t i = 0; i < merchants.size(); ++i) {
        if (merchants[i] != npc_tile_id)
            continue;

        npc_id = npc_tile_id;
        return true;
    }

    return false;
}

const bool Game::_validatePortalPosition(const InstanceId caller,
                                         const uint32_t x_coord,
                                         const uint32_t y_coord) {
    Id map_id = this->characters.at(caller).getMapId();
    if (this->logic_maps.getTile(map_id, x_coord, y_coord).portal)
        return true;

    return false;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Métodos auxiliares para la ejecución de los comandos
//-----------------------------------------------------------------------------

const bool Game::_validatePortalMapId(const Id map_id) {
    std::vector<Id> maps_id;
    this->logic_maps.getMapsId(maps_id);

    std::vector<Id>::iterator it =
        std::find(maps_id.begin(), maps_id.end(), map_id);

    if (it == maps_id.end())
        return false;

    return true;
}

const bool Game::_validateIfNPCMarketsItem(const Id npc_id, const Id item_id) {
    const std::list<Id>& sellable_items = npcs[npc_id].sellable_items;
    std::list<Id>::const_iterator it =
        std::find(sellable_items.begin(), sellable_items.end(), item_id);

    if (it == sellable_items.end())
        return false;

    return true;
}

void Game::_listPortalMaps(std::string& init_msg,
                           std::list<std::string>& item_list) {
    init_msg = "Posibles mapas para viajar";

    uint32_t size = this->logic_maps.getMapsSize();

    for (Id id = 0; id < size; ++id) {
        item_list.push_back(std::to_string(id) + ": " +
                            this->logic_maps.getMapName(id));
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

const bool Game::_dropItem(const InstanceId caller, const uint8_t n_slot,
                           uint32_t& amount, Item** dropped) {
    Character& character = this->characters.at(caller);

    *dropped = character.dropItem(n_slot, amount);

    return (*dropped != nullptr);
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
            this->logic_maps.addItem(map_id, dropped_items[i].item,
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

void Game::_useWeapon(const InstanceId caller, const InstanceId target,
                      Attackable* attacked, const bool target_is_creature) {
    Character& attacker = this->characters.at(caller);
    const unsigned int prev_level = attacker.getLevel();

    int damage = 0;
    bool eluded = false;

    Response attack_response = attacker.useWeapon(attacked, damage, eluded);

    if (!attack_response.succeeded) {
        _notifyResponse(caller, attack_response);
        return;
    }

    // Verificamos si murió, en cuyo caso dropea todo.
    if (target_is_creature)
        _sendCreatureAttackNotifications(damage, caller, target);
    else
        _sendCharacterAttackNotifications(damage, eluded, caller, target);

    // Ante la muerte del atacado
    if (damage > 0 && !attacked->getHealth()) {
        _dropAllItems(attacked);

        if (target_is_creature)
            deleteCreature(target);
        else
            _pushCharacterEvent(target, DEATH_EV_TYPE);
    }

    if (prev_level < attacker.getLevel())
        _pushCharacterEvent(caller, LEVEL_UP_EV_TYPE);
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
        this->logic_maps.getNearestFreeTile(map_id, respawn_x, respawn_y,
                                            false);
    } catch (const CouldNotFindFreeTileException& e) {
        // No puede teletransportarse, entonces se queda en su posición actual.
        _pushCharacterDifferentialBroadcast(caller, UPDATE_BROADCAST, true);
        _notifyResurrection(caller);
        return;
    }

    this->logic_maps.clearTileOccupant(map_id, character.getPosition().getX(),
                                       character.getPosition().getY());
    this->logic_maps.occupyTile(map_id, caller, respawn_x, respawn_y);
    character.teleport(map_id, respawn_x, respawn_y);

    _pushCharacterDifferentialBroadcast(caller, UPDATE_BROADCAST, true);
    _notifyResurrection(caller);
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

        switch (weapon_type) {
            case EXPLOSIVE: {
                _pushCharacterEvent(target, EXPLOSION_SPELL_EV_TYPE);
                break;
            }

            case FIRE: {
                _pushCharacterEvent(target, FIRE_SPELL_EV_TYPE);
                break;
            }

            case LIGHTNING: {
                _pushCharacterEvent(target, LIGHTNING_SPELL_EV_TYPE);
                break;
            }

            default: {
                _pushCharacterEvent(caller, GRAL_ATTACK_EV_TYPE);
                break;
            }
        }

        _pushCharacterMainEvent(target, BEATTACKED_EV_TYPE);
        _pushCharacterEvent(target, DMG_EV_TYPE);

    } else if (damage <= 0 && weapon_type == HEALING) {
        msg_to_attacker =
            "Le has curado " + std::to_string(-damage) + " puntos de vida.";
        msg_to_attacked =
            "Te han curado " + std::to_string(-damage) + " puntos de vida.";

        _pushCharacterEvent(target, HEALING_SPELL_EV_TYPE);
        _pushCharacterMainEvent(target, BEHEALED_EV_TYPE);

    } else if (eluded) {
        msg_to_attacker = "Tu ataque fue eludido.";
        msg_to_attacked = "Has eludido un ataque.";
        _pushCharacterEvent(target, EVASION_EV_TYPE);
    } else {
        msg_to_attacker =
            "No le has causado daño, la defensa absorbió el ataque.";
        msg_to_attacked = "Tu defensa absorbió todo el daño del ataque.";

        // Provisorio, conviene ponerlo en un evento aparte.
        switch (weapon_type) {
            case EXPLOSIVE: {
                _pushCharacterEvent(target, EXPLOSION_SPELL_EV_TYPE);
                break;
            }

            case FIRE: {
                _pushCharacterEvent(target, FIRE_SPELL_EV_TYPE);
                break;
            }

            case LIGHTNING: {
                _pushCharacterEvent(target, LIGHTNING_SPELL_EV_TYPE);
                break;
            }

            default: {
                _pushCharacterEvent(caller, GRAL_ATTACK_EV_TYPE);
                break;
            }
        }
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

    switch (weapon_type) {
        case EXPLOSIVE: {
            _pushCreatureEvent(target, EXPLOSION_SPELL_EV_TYPE);
            break;
        }

        case FIRE: {
            _pushCreatureEvent(target, FIRE_SPELL_EV_TYPE);
            break;
        }

        case LIGHTNING: {
            _pushCreatureEvent(target, LIGHTNING_SPELL_EV_TYPE);
            break;
        }

        default: {
            _pushCharacterEvent(caller, GRAL_ATTACK_EV_TYPE);
            break;
        }
    }

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

void Game::_notifyResurrection(const InstanceId caller) {
    _notifyResponse(caller,
                    Response(true, RESURRECTED_SUCCESS_MSG, SUCCESS_MSG));
    _pushCharacterEvent(caller, RESURRECT_EV_TYPE);
    _pushCharacterMainEvent(caller, BEHEALED_EV_TYPE);
}

void Game::_notifyResponse(const InstanceId caller, const Response& response) {
    if (!response.msg.length())
        return;

    Notification* reply = new Reply(response.message_type, response.msg);
    active_clients.notify(caller, reply);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Creación y eliminación de entidades
//----------------------------------------------------------------------------

const InstanceId Game::newCharacter(const CharacterCfg& init_data) {
    Id new_character_id = this->next_instance_id;
    ++this->next_instance_id;

    Id spawning_map_id = init_data.map;
    int spawning_x_coord, spawning_y_coord;

    _establishCharacterSpawningPosition(init_data, spawning_map_id,
                                        spawning_x_coord, spawning_y_coord);

    this->logic_maps.occupyTile(spawning_map_id, new_character_id,
                                spawning_x_coord, spawning_y_coord);

    this->characters.emplace(
        std::piecewise_construct, std::forward_as_tuple(new_character_id),
        std::forward_as_tuple(
            init_data, this->races[init_data.race], this->kinds[init_data.kind],
            logic_maps, spawning_map_id, spawning_x_coord, spawning_y_coord,
            this->items, formulas, rate, cfg.critical_attack_dmg_modifier,
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

    this->logic_maps.establishEntitySpawningPosition(init_map, spawning_x_coord,
                                                     spawning_y_coord, true);

    this->logic_maps.occupyTile(init_map, new_creature_id, spawning_x_coord,
                                spawning_y_coord);

    this->creatures.emplace(
        std::piecewise_construct, std::forward_as_tuple(new_creature_id),
        std::forward_as_tuple(init_data, logic_maps, init_map, spawning_x_coord,
                              spawning_y_coord, init_data.base_health, items,
                              characters, *this, formulas, rate,
                              cfg.random_movement_factor));

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
    --(this->maps_creatures_info.at(creature.getMapId()).amount_of_creatures);

    this->creatures.erase(id);
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

        _notifyResponse(id, character.act(it));

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

        if (creature.mustBeBroadcasted())
            _pushCreatureDifferentialBroadcast(id, UPDATE_BROADCAST);

        InstanceId receiver = creature.getAttackingCharacterId();

        if (receiver) {
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
                this->logic_maps.clearTileItem(map_iterator->first, x, y);
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

/******************************************************************************
 *                                COMANDOS                                    *
 *****************************************************************************/

/*****************************************************************************/
/*****************************************************************************/

//-----------------------------------------------------------------------------
// Movimiento
//-----------------------------------------------------------------------------

void Game::startMovingUp(const InstanceId caller) {
    Character& character = this->characters.at(caller);

    _notifyResponse(caller, character.startMovingUp());
}

void Game::startMovingDown(const InstanceId caller) {
    Character& character = this->characters.at(caller);

    _notifyResponse(caller, character.startMovingDown());
}

void Game::startMovingLeft(const InstanceId caller) {
    Character& character = this->characters.at(caller);

    _notifyResponse(caller, character.startMovingLeft());
}

void Game::startMovingRight(const InstanceId caller) {
    Character& character = this->characters.at(caller);

    _notifyResponse(caller, character.startMovingRight());
}

void Game::stopMoving(const InstanceId caller) {
    Character& character = this->characters.at(caller);

    character.stopMoving();
}

void Game::teleport(const InstanceId caller, const uint32_t portal_x_coord,
                    const uint32_t portal_y_coord, const Id map_id) {
    Character& character = this->characters.at(caller);

    // Si está resucitando, no se puede teletransportar.
    if (this->resurrecting_players_cooldown.count(caller)) {
        _notifyResponse(
            caller,
            Response(false, TELEPORT_WHILE_RESURRECTING_ERROR_MSG, ERROR_MSG));
        return;
    }

    if (!_validatePortalPosition(caller, portal_x_coord, portal_y_coord)) {
        _notifyResponse(
            caller,
            Response(false, INVALID_PORTAL_COORDINATES_ERROR_MSG, ERROR_MSG));
        return;
    }

    if (map_id == character.getMapId()) {
        _notifyResponse(
            caller,
            Response(false, TELEPORT_TO_ACTUAL_MAP_ERROR_MSG, ERROR_MSG));
        return;
    }

    if (!_validatePortalMapId(map_id)) {
        _notifyResponse(caller, Response(false, INVALID_PORTAL_MAP_ID_ERROR_MSG,
                                         ERROR_MSG));
        return;
    }

    _pushCharacterDifferentialBroadcast(caller, DELETE_BROADCAST, false);

    int spawning_x_coord, spawning_y_coord;
    this->logic_maps.establishEntitySpawningPosition(map_id, spawning_x_coord,
                                                     spawning_y_coord, false);
    this->logic_maps.occupyTile(map_id, caller, spawning_x_coord,
                                spawning_y_coord);

    this->logic_maps.clearTileOccupant(character.getMapId(),
                                       character.getPosition().getX(),
                                       character.getPosition().getY());

    character.teleport(map_id, spawning_x_coord, spawning_y_coord);
    active_clients.changeMap(caller, map_id);

    _pushCharacterDifferentialBroadcast(caller, NEW_BROADCAST, false);

    // este podria ser reemplazado por uno solo hacia el caller
    _pushCharacterDifferentialBroadcast(caller, UPDATE_BROADCAST, true);

    _pushFullBroadcast(caller, false);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Manipulación de items
//-----------------------------------------------------------------------------

void Game::equip(const InstanceId caller, const uint8_t n_slot) {
    Character& character = this->characters.at(caller);

    _notifyResponse(caller, character.equip(n_slot));
}

void Game::unequip(const InstanceId caller, const uint8_t n_slot) {
    Character& character = this->characters.at(caller);

    _notifyResponse(caller, character.unequip(n_slot));
}

void Game::take(const InstanceId caller) {
    Character& character = this->characters.at(caller);
    Id map_id = character.getMapId();
    int x_coord = character.getPosition().getX();
    int y_coord = character.getPosition().getY();

    const Tile& tile = this->logic_maps.getTile(map_id, x_coord, y_coord);

    Id item_id = tile.item_id;
    uint32_t amount = tile.item_amount;

    if (!item_id)
        return;  // No hay item droppeado en su posición.

    Response response = character.takeItem(this->items[item_id], amount);
    _notifyResponse(caller, response);

    if (!response.succeeded)
        return;

    this->logic_maps.clearTileItem(map_id, x_coord, y_coord);

    _pushItemDifferentialBroadcast(map_id, x_coord, y_coord, DELETE_BROADCAST);

    _pushCharacterEvent(caller, GRAB_EV_TYPE);
}

void Game::drop(const InstanceId caller, const uint8_t n_slot,
                uint32_t amount) {
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
        this->logic_maps.addItem(map_id, dropped_item_id, amount, x, y);
    } catch (const CouldNotFindFreeTileException& e) {
        // No se pudo efectuar el dropeo. Le devuelvo el item al character.
        character.takeItem(dropped, amount);
        return;
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

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Recuperación de vida/maná
//-----------------------------------------------------------------------------

void Game::heal(const InstanceId caller, const uint32_t x_coord,
                const uint32_t y_coord) {
    Id priest_id;

    if (!_validatePriestPosition(caller, priest_id, x_coord, y_coord)) {
        _notifyResponse(
            caller,
            Response(false, INVALID_PRIEST_COORDINATES_ERROR_MSG, ERROR_MSG));
        return;
    }

    Character& character = this->characters.at(caller);

    Response response = character.heal();

    _notifyResponse(caller, response);

    if (!response.succeeded)
        return;

    _pushCharacterEvent(caller, HEALED_BY_PRIEST_EV_TYPE);
    _pushCharacterMainEvent(caller, BEHEALED_EV_TYPE);
}

void Game::meditate(const InstanceId caller) {
    Character& character = this->characters.at(caller);

    Response response = character.meditate();

    _notifyResponse(caller, response);

    if (!response.succeeded)
        return;

    _pushCharacterEvent(caller, MEDITATE_EV_TYPE);
}

void Game::resurrect(const InstanceId caller) {
    Character& character = this->characters.at(caller);

    if (this->resurrecting_players_cooldown.count(caller)) {
        std::string msg =
            "Ya estás resucitando. Sé paciente. Todavía debes esperar " +
            std::to_string(
                this->resurrecting_players_cooldown.at(caller).cooldown /
                1000) +
            " segundos.";
        _notifyResponse(caller, Response(false, msg, ERROR_MSG));
        return;
    }

    Response response = character.startResurrecting();
    if (!response.succeeded) {
        _notifyResponse(caller, response);
        return;
    }

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
    _notifyResponse(caller, Response(true, reply_msg, INFO_MSG));
}

void Game::resurrect(const InstanceId caller, const uint32_t x_coord,
                     const uint32_t y_coord) {
    if (this->resurrecting_players_cooldown.count(caller)) {
        std::string msg =
            "Ya estás resucitando. Sé paciente. Todavía debes esperar " +
            std::to_string(
                this->resurrecting_players_cooldown.at(caller).cooldown /
                1000) +
            " segundos.";
        _notifyResponse(caller, Response(false, msg, ERROR_MSG));
        return;
    }

    Id priest_id;

    if (!_validatePriestPosition(caller, priest_id, x_coord, y_coord)) {
        _notifyResponse(
            caller,
            Response(false, INVALID_PRIEST_COORDINATES_ERROR_MSG, ERROR_MSG));
        return;
    }

    Character& character = this->characters.at(caller);

    Response response = character.resurrect();

    if (!response.succeeded) {
        _notifyResponse(caller, response);
        return;
    }

    _notifyResurrection(caller);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Interacciones entre jugadores
//-----------------------------------------------------------------------------

void Game::useWeapon(const InstanceId caller, const InstanceId target) {
    if (this->characters.count(target)) {
        Character& attacked = this->characters.at(target);
        _useWeapon(caller, target, &attacked, false);
    } else if (this->creatures.count(target)) {
        Creature& attacked = this->creatures.at(target);
        _useWeapon(caller, target, &attacked, true);
    } else {
        _notifyResponse(caller, Response(false, INVALID_ATTACK_TARGET_ERROR_MSG,
                                         ERROR_MSG));
    }
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Interacción con el banco
//-----------------------------------------------------------------------------

void Game::depositItemOnBank(const InstanceId caller, const uint32_t x_coord,
                             const uint32_t y_coord, const uint8_t n_slot,
                             uint32_t amount) {
    Id banker_id;

    if (!_validateBankerPosition(caller, banker_id, x_coord, y_coord)) {
        _notifyResponse(
            caller,
            Response(false, INVALID_BANKER_COORDINATES_ERROR_MSG, ERROR_MSG));
        return;
    }

    uint32_t asked_amount = amount;
    Item* to_deposit = nullptr;

    if (!_dropItem(caller, n_slot, amount, &to_deposit))
        return;

    if (amount < asked_amount)
        _notifyResponse(
            caller,
            Response(false, INSUFFICIENT_AMOUNT_OF_ITEMS_ERROR_MSG, INFO_MSG));

    Character& character = this->characters.at(caller);
    BankAccount& account = this->bank[character.getNickname()];

    Response response = account.deposit(to_deposit->getId(), amount);

    if (!response.succeeded)
        character.takeItem(to_deposit, amount);

    _notifyResponse(caller, response);
}

void Game::withdrawItemFromBank(const InstanceId caller, const uint32_t x_coord,
                                const uint32_t y_coord, const uint32_t item_id,
                                uint32_t amount) {
    Id banker_id;

    if (!_validateBankerPosition(caller, banker_id, x_coord, y_coord)) {
        _notifyResponse(
            caller,
            Response(false, INVALID_BANKER_COORDINATES_ERROR_MSG, ERROR_MSG));
        return;
    }

    uint32_t asked_amount = amount;
    Item* withdrew = nullptr;

    Character& character = this->characters.at(caller);
    BankAccount& account = this->bank[character.getNickname()];

    withdrew = account.withdraw(item_id, amount);

    if (!withdrew) {
        _notifyResponse(
            caller,
            Response(false, BANK_ACCOUNT_INVALID_ITEM_ERROR_MSG, ERROR_MSG));
        return;
    }

    Response response = character.takeItem(withdrew, amount);

    if (!response.succeeded) {
        account.deposit(withdrew->getId(), amount);
        _notifyResponse(caller, response);
        return;
    }

    std::string reply_msg = "";

    if (asked_amount > amount)
        _notifyResponse(
            caller,
            Response(false, INSUFFICIENT_AMOUNT_OF_ITEMS_ERROR_MSG, INFO_MSG));

    reply_msg += "Se ha extraído " + withdrew->what() + " x" +
                 std::to_string(amount) + " del banco";

    _notifyResponse(caller, Response(true, reply_msg, SUCCESS_MSG));
}

void Game::depositGoldOnBank(const InstanceId caller, const uint32_t x_coord,
                             const uint32_t y_coord, const uint32_t amount) {
    Id banker_id;

    if (!_validateBankerPosition(caller, banker_id, x_coord, y_coord)) {
        _notifyResponse(
            caller,
            Response(false, INVALID_BANKER_COORDINATES_ERROR_MSG, ERROR_MSG));
        return;
    }

    Character& character = this->characters.at(caller);

    Response response = character.gatherGold(amount);

    if (!response.succeeded) {
        _notifyResponse(caller, response);
        return;
    }

    BankAccount& account = this->bank[character.getNickname()];

    _notifyResponse(caller, account.depositGold(amount));
}

void Game::withdrawGoldFromBank(const InstanceId caller, const uint32_t x_coord,
                                const uint32_t y_coord, uint32_t amount) {
    Id banker_id;

    if (!_validateBankerPosition(caller, banker_id, x_coord, y_coord)) {
        _notifyResponse(
            caller,
            Response(false, INVALID_BANKER_COORDINATES_ERROR_MSG, ERROR_MSG));
        return;
    }

    Character& character = this->characters.at(caller);
    BankAccount& account = this->bank[character.getNickname()];

    Response bank_response = account.withdrawGold(amount);
    if (!bank_response.succeeded) {
        _notifyResponse(caller, bank_response);
        return;
    }

    uint32_t taken_amount = amount;

    Response taken_gold_response = character.takeGold(taken_amount);

    if (!taken_gold_response.succeeded) {
        _notifyResponse(caller, taken_gold_response);
        account.depositGold(amount);
        return;
    }

    _notifyResponse(caller, bank_response);
    _notifyResponse(caller, taken_gold_response);

    // Si llegó al máximo de oro en el inventario, depositamos nuevamente lo que
    // no pudo tomar.
    if (amount - taken_amount > 0)
        _notifyResponse(caller, account.depositGold(amount - taken_amount));
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Interacción con compradores/vendedores (comerciantes y banqueros)
//-----------------------------------------------------------------------------

void Game::buyItem(const InstanceId caller, const uint32_t x_coord,
                   const uint32_t y_coord, const uint32_t item_id,
                   const uint32_t amount) {
    Character& character = this->characters.at(caller);

    // Verifico x e y correspondan a la posición de un comerciante o sacerdote.
    Id npc_id = 0;

    if (!_validatePriestPosition(caller, npc_id, x_coord, y_coord) &&
        !_validateMerchantPosition(caller, npc_id, x_coord, y_coord)) {
        _notifyResponse(
            caller,
            Response(false, INVALID_SELLER_COORDINATES_ERROR_MSG, ERROR_MSG));
        return;
    }

    if (!_validateIfNPCMarketsItem(npc_id, item_id)) {
        _notifyResponse(
            caller,
            Response(false, INVALID_MARKETING_ITEM_ERROR_MSG, ERROR_MSG));
        return;
    }

    unsigned int total_price = items[item_id]->getPrice() * amount;

    Response gathering_response = character.gatherGold(total_price);
    if (!gathering_response.succeeded) {
        _notifyResponse(caller, gathering_response);
        return;
    }

    Response taking_response = character.takeItem(this->items[item_id], amount);

    if (!taking_response.succeeded) {
        character.takeGold(total_price);
        _notifyResponse(caller, taking_response);
        return;
    }

    std::string reply_msg =
        "Has comprado " + std::to_string(amount) + " " + items[item_id]->what();
    _notifyResponse(caller, Response(true, reply_msg, SUCCESS_MSG));
}

void Game::sellItem(const InstanceId caller, const uint32_t x_coord,
                    const uint32_t y_coord, const uint8_t n_slot,
                    uint32_t amount) {
    Character& character = this->characters.at(caller);

    // Verifico x e y correspondan a la posición de un comerciante o sacerdote.
    Id npc_id = 0;

    if (!_validatePriestPosition(caller, npc_id, x_coord, y_coord) &&
        !_validateMerchantPosition(caller, npc_id, x_coord, y_coord)) {
        _notifyResponse(
            caller,
            Response(false, INVALID_SELLER_COORDINATES_ERROR_MSG, ERROR_MSG));
        return;
    }

    Item* to_sell = nullptr;

    if (!_dropItem(caller, n_slot, amount, &to_sell))
        return;

    if (!_validateIfNPCMarketsItem(npc_id, to_sell->getId())) {
        character.takeItem(to_sell, amount);
        _notifyResponse(
            caller,
            Response(false, INVALID_MARKETING_ITEM_ERROR_MSG, ERROR_MSG));
        return;
    }

    unsigned int total_price = to_sell->getPrice() * amount;
    unsigned int taken_amount = total_price;

    Response gold_taking_response = character.takeGold(taken_amount);

    if (!gold_taking_response.succeeded) {
        unsigned int amount_to_replenish =
            (total_price - taken_amount) / to_sell->getPrice();
        if (amount_to_replenish)
            character.takeItem(to_sell, amount_to_replenish);
        _notifyResponse(caller, gold_taking_response);
        return;
    }

    // si amount < asked_amount falta avisar.

    std::string reply_msg = "Has vendido " +
                            std::to_string(taken_amount / to_sell->getPrice()) +
                            " " + to_sell->what();
    _notifyResponse(caller, Response(true, reply_msg, SUCCESS_MSG));
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Servicio de mensajería
//-----------------------------------------------------------------------------

void Game::sendPrivateMessage(const InstanceId caller,
                              const std::string to_nickname,
                              const std::string message) {
    if (!this->nickname_id_map.count(to_nickname)) {
        _notifyResponse(caller,
                        Response(false, INVALID_NICKNAME_ERROR_MSG, ERROR_MSG));
        return;
    }

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
    const std::string& caller_nickname =
        this->characters.at(caller).getNickname();

    Notification* notification =
        new Message(caller_nickname, message, GENERAL_MSG, caller);
    this->active_clients.sendMessageToAll(notification);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Comandos informativos
//-----------------------------------------------------------------------------

void Game::list(const InstanceId caller, const uint32_t x_coord,
                const uint32_t y_coord) {
    Character& character = this->characters.at(caller);

    Id npc_id;

    std::string init_msg;
    std::list<std::string> list_items;

    if (_validateBankerPosition(caller, npc_id, x_coord, y_coord)) {
        BankAccount& account = bank[character.getNickname()];
        account.list(init_msg, list_items);
    } else if (_validatePriestPosition(caller, npc_id, x_coord, y_coord) ||
               _validateMerchantPosition(caller, npc_id, x_coord, y_coord)) {
        _listNPCSellableItems(npc_id, init_msg, list_items);
    } else if (_validatePortalPosition(caller, x_coord, y_coord)) {
        _listPortalMaps(init_msg, list_items);
    } else {
        _notifyResponse(caller,
                        Response(false, NO_LISTABLE_NPC_ERROR_MSG, ERROR_MSG));
        return;
    }

    Notification* list = new List(init_msg, list_items);
    this->active_clients.notify(caller, list);
    return;
}

void Game::help(const InstanceId caller, const uint32_t x_coord,
                const uint32_t y_coord) {
    std::string init_msg;
    std::list<std::string> descriptions;

    Id npc_id = 0;

    if (_validatePriestPosition(caller, npc_id, x_coord, y_coord)) {
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
    } else if (_validateBankerPosition(caller, npc_id, x_coord, y_coord)) {
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
    } else if (_validateMerchantPosition(caller, npc_id, x_coord, y_coord)) {
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
        _notifyResponse(
            caller, Response(false, NO_HELP_NPC_SELECTED_ERROR_MSG, ERROR_MSG));
        return;
    }

    Notification* list = new List(init_msg, descriptions);
    active_clients.notify(caller, list);
}

void Game::listConnectedPlayers(const InstanceId caller) {
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

//-----------------------------------------------------------------------------

/*****************************************************************************/
/*****************************************************************************/

//-----------------------------------------------------------------------------
// Interacción con criaturas
//-----------------------------------------------------------------------------

void Game::beAttackedByCreature(const InstanceId caller, int& damage,
                                bool eluded) {
    Character& attacked = this->characters.at(caller);

    if (damage > 0 && !attacked.getHealth()) {
        _pushCharacterEvent(caller, DEATH_EV_TYPE);
        _dropAllItems(&attacked);
    }

    _sendAttackedByCreatureNotifications(damage, eluded, caller);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Obtención de atributos
//-----------------------------------------------------------------------------

const Id Game::getMapId(const InstanceId caller) {
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
