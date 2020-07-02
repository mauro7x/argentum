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
#include "../../includes/Control/Notifications/ItemBroadcast.h"
#include "../../includes/Control/Notifications/List.h"
#include "../../includes/Control/Notifications/Message.h"
#include "../../includes/Control/Notifications/Reply.h"
//-----------------------------------------------------------------------------
#include "../../includes/Model/Game.h"
//-----------------------------------------------------------------------------
#define FIRST_INSTANCE_ID 1
//-----------------------------------------------------------------------------

Game::Game(ActiveClients& active_clients)
    : bank(items),
      data_persistence_cooldown(TIME_TO_PERSIST_DATA),
      next_instance_id(FIRST_INSTANCE_ID),
      active_clients(active_clients) {
    map_container.loadMaps();

    std::vector<Id> maps_id = std::move(this->map_container.getMapsId());

    for (unsigned int i = 0; i < maps_id.size(); ++i) {
        this->maps_creatures_info.emplace(
            std::piecewise_construct, std::forward_as_tuple(maps_id[i]),
            std::forward_as_tuple(0, TIME_TO_SPAWN_CREATURE));
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
//-----------------------------------------------------------------------------
// Métodos auxiliares de creacion de entidades;
//-----------------------------------------------------------------------------
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

    fprintf(stderr, "deberia espawnear en map: %i, x: %i, y: %i\n",
            init_data.map, init_data.x_tile, init_data.y_tile);

    const Id spawning_map_id = this->map_container.getCharacterSpawningMap();
    int spawning_x_coord;
    int spawning_y_coord;
    this->map_container[spawning_map_id].establishEntitySpawningPosition(
        new_character_id, spawning_x_coord, spawning_y_coord, false);

    this->characters.emplace(
        std::piecewise_construct, std::forward_as_tuple(new_character_id),
        std::forward_as_tuple(init_data, this->races[init_data.race],
                              this->kinds[init_data.kind], this->map_container,
                              spawning_map_id, spawning_x_coord,
                              spawning_y_coord, this->items));

    this->nickname_id_map[init_data.nickname] = new_character_id;

    this->characters.at(new_character_id).debug();

    _loadBankAccount(init_data);

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
                              init_data.base_health, init_data.base_damage,
                              items, characters));

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

const Id Game::_randomSelectCreature() const {
    std::vector<Id> possible_creatures;

    this->creatures_data.gatherIds(possible_creatures);

    RandomNumberGenerator gen;
    return possible_creatures[gen(0, possible_creatures.size() - 1)];
}

void Game::_spawnNewCreature(const Id spawning_map) {
    const Id creature_id = _randomSelectCreature();

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
        try {
            it_characters->second.act(it);
        } catch (const CollisionWhileMovingException& e) {
            it_characters->second.stopMoving();
            Notification* reply = new Reply(ERROR_MSG, e.what());
            active_clients.notify(it_characters->first, reply);
        }

        if (it_characters->second.mustBeBroadcasted()) {
            _pushCharacterDifferentialBroadcast(it_characters->first,
                                                UPDATE_BROADCAST, true);
        }

        // it_characters->second.debug();

        ++it_characters;
    }
}

void Game::actCreatures(const int it) {
    std::unordered_map<InstanceId, Creature>::iterator it_creatures =
        this->creatures.begin();

    while (it_creatures != this->creatures.end()) {
        try {
            it_creatures->second.act(it);
        } catch (const CollisionWhileMovingException& e) {
            it_creatures->second.stopMoving();
        }

        if (it_creatures->second.mustBeBroadcasted()) {
            _pushCreatureDifferentialBroadcast(it_creatures->first,
                                               UPDATE_BROADCAST);
        }

        // it_creatures->second.debug();

        ++it_creatures;
    }
}

void Game::spawnNewCreatures(const int it) {
    std::unordered_map<Id, MapCreaturesInfo>::iterator iterator =
        this->maps_creatures_info.begin();

    while (iterator != this->maps_creatures_info.end()) {
        if (iterator->second.amount_of_creatures == MAX_CREATURES_PER_MAP) {
            ++iterator;
            continue;
        }

        iterator->second.creature_spawning_cooldown -= it * RATE;
        while (iterator->second.creature_spawning_cooldown <= 0) {
            _spawnNewCreature(iterator->first);
            ++iterator->second.amount_of_creatures;
            iterator->second.creature_spawning_cooldown +=
                TIME_TO_SPAWN_CREATURE;
        }

        ++iterator;
    }
}

void Game::persistPeriodicData(Database& database, const int it) {
    std::unordered_map<InstanceId, Character>::iterator it_characters =
        this->characters.begin();
    data_persistence_cooldown -= it * RATE;
    if (data_persistence_cooldown <= 0) {
        while (it_characters != this->characters.end()) {
            CharacterCfg character_data = {};
            bank[it_characters->second.getNickname()].fillPersistenceData(
                character_data);
            it_characters->second.fillPersistenceData(character_data);
            database.persistPlayerData(character_data, false);
            ++it_characters;
        }
        data_persistence_cooldown = TIME_TO_PERSIST_DATA;
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

            lifetime -= it * RATE;

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

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Comandos
//-----------------------------------------------------------------------------

void Game::startMovingUp(const InstanceId caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::startMovingUp: unknown caller.");
    }

    Character& character = this->characters.at(caller);
    character.startMovingUp();
}

void Game::startMovingDown(const InstanceId caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::startMovingDown: unknown caller.");
    }

    Character& character = this->characters.at(caller);
    character.startMovingDown();
}

void Game::startMovingLeft(const InstanceId caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::startMovingLeft: unknown caller.");
    }

    Character& character = this->characters.at(caller);
    character.startMovingLeft();
}

void Game::startMovingRight(const InstanceId caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::startMovingRight: unknown caller.");
    }

    Character& character = this->characters.at(caller);
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
        } catch (const ItemCouldNotBeAddedException& e) {
            // No se pudo efectuar el dropeo. Cancelo.
            return;
        }

        // Agrego elemento al mapa de dropped items cooldown
        const std::string key = std::move(_coordinatesToMapKey(x, y));
        this->dropped_items_lifetime_per_map[map_id].emplace(
            key, TIME_TO_DISSAPEAR_DROPPED_ITEM);

        // Broadcasteo new item
        _pushItemDifferentialBroadcast(map_id, x, y, NEW_BROADCAST);
    }
}

void Game::_sendCharacterAttackNotifications(const int damage,
                                             const bool eluded,
                                             const InstanceId caller,
                                             const InstanceId target) {
    std::string msg_to_attacked, msg_to_attacker;

    if (damage > 0) {
        msg_to_attacker =
            "Le has infligido " + std::to_string(damage) + " de daño.";
        msg_to_attacked =
            "Has recibido " + std::to_string(damage) + " de daño.";
    } else if (damage < 0) {
        msg_to_attacker =
            "Le has curado " + std::to_string(-damage) + " puntos de vida.";
        msg_to_attacked =
            "Te han curado " + std::to_string(-damage) + " puntos de vida.";
    } else if (eluded) {
        msg_to_attacker = "Tu ataque fue eludido.";
        msg_to_attacked = "Has eludido un ataque.";
    } else {
        msg_to_attacker =
            "No le has causado daño, la defensa absorbió el ataque.";
        msg_to_attacked = "Tu defensa absorbió todo el daño del ataque.";
    }

    Notification* reply = new Reply(INFO_MSG, msg_to_attacker.c_str());
    active_clients.notify(caller, reply);

    if (caller == target)
        return;

    reply = new Reply(INFO_MSG, msg_to_attacked.c_str());
    active_clients.notify(target, reply);
}

void Game::_sendCreatureAttackNotifications(const int damage,
                                            const InstanceId caller) {
    std::string msg_to_attacker = "Has atacado a la criatura, provocando " +
                                  std::to_string(damage) + " de daño.";

    Notification* reply = new Reply(INFO_MSG, msg_to_attacker.c_str());
    active_clients.notify(caller, reply);
}

void Game::_useWeapon(const InstanceId caller, const InstanceId target,
                      Attackable* attacked, const bool target_is_creature) {
    Character& attacker = this->characters.at(caller);

    int damage = 0;
    bool eluded = false;

    try {
        eluded = attacker.useWeapon(attacked, damage);
    } catch (const std::exception& e) {
        /*
         * Atrapo excepciones:
         * OutOfRangeAttackException,
         * CantAttackWithoutWeaponException,
         * KindCantDoMagicException,
         * TooHighLevelDifferenceOnAttackException,
         * NewbiesCantBeAttackedException, InsufficientManaException,
         * AttackedActualStateCantBeAttackedException,
         * AttackCooldownTimeNotElapsedException,
         * CantAttackItselfException
         */
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
        return;
    }

    // Verificamos si murió, en cuyo caso dropea todo.
    if (damage > 0 && !attacked->getHealth()) {
        _dropAllItems(attacked);

        if (target_is_creature)
            deleteCreature(target);
    }

    if (target_is_creature)
        _sendCreatureAttackNotifications(damage, caller);
    else
        _sendCharacterAttackNotifications(damage, eluded, caller, target);
}

void Game::useWeapon(const InstanceId caller, const InstanceId target) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::useWeapon: unknown caller.");
    }

    if (this->characters.count(target)) {
        Character& attacked = this->characters.at(target);
        _useWeapon(caller, target, &attacked, false);
        return;
    }

    if (this->creatures.count(target)) {
        Creature& attacked = this->creatures.at(target);
        _useWeapon(caller, target, &attacked, true);
        return;
    }

    // Excepcion. target invalido.
}

void Game::equip(const InstanceId caller, const uint8_t n_slot) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::equip: unknown caller.");
    }

    Character& character = this->characters.at(caller);

    try {
        character.equip(n_slot);
    } catch (const std::exception& e) {
        // InvalidInventorySlotNumberException,
        // KindCantDoMagicException,
        // PotionHasNoPointsToRecoverException
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
        return;
    }
}

void Game::unequip(const InstanceId caller, const uint8_t n_slot) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::unequip: unknown caller.");
    }

    Character& character = this->characters.at(caller);

    try {
        character.unequip(n_slot);
    } catch (const std::exception& e) {
        // FullInventoryException e InvalidEquipmentSlotNumberException
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
        return;
    }
}

void Game::meditate(const InstanceId caller) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::unequip: unknown caller.");
    }

    Character& character = this->characters.at(caller);

    try {
        character.meditate();
    } catch (const KindCantMeditateException& e) {
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
        return;
    }

    std::string reply_msg =
        "Has comenzado a meditar. Ante cualquier acción dejarás de hacerlo.";
    Notification* reply = new Reply(SUCCESS_MSG, reply_msg);
    active_clients.notify(caller, reply);
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

    try {
        character.takeItem(this->items[item_id], amount);
    } catch (const std::exception& e) {
        // FullInventoryException, StateCantTakeItemException,
        // GoldMaximumCapacityReached.
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
        return;
    }

    this->map_container[map_id].clearTileItem(x_coord, y_coord);
    _pushItemDifferentialBroadcast(map_id, x_coord, y_coord, DELETE_BROADCAST);
}

const bool Game::_dropItem(const InstanceId caller, const uint8_t n_slot,
                           uint32_t& amount, Item** dropped) {
    Character& character = this->characters.at(caller);

    try {
        *dropped = character.dropItem(n_slot, amount);
    } catch (const InvalidInventorySlotNumberException& e) {
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
        return false;
    }

    return (*dropped != nullptr);
}

void Game::drop(const InstanceId caller, const uint8_t n_slot,
                uint32_t amount) {
    if (!this->characters.count(caller)) {
        throw Exception("Game::equip: unknown caller.");
    }

    uint32_t asked_amount = amount;
    Item* dropped = nullptr;

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
    } catch (const ItemCouldNotBeAddedException& e) {
        // No se pudo efectuar el dropeo. Le devuelvo el item al character.
        character.takeItem(dropped, amount);
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
        return;
    }

    // Agrego elemento al mapa de dropped items cooldown
    const std::string key = std::move(_coordinatesToMapKey(x, y));
    this->dropped_items_lifetime_per_map[map_id].emplace(
        key, TIME_TO_DISSAPEAR_DROPPED_ITEM);

    // Broadcasteo new item
    _pushItemDifferentialBroadcast(map_id, x, y, NEW_BROADCAST);
}

void Game::resurrect(const InstanceId caller) {
    fprintf(stderr, "Game::resurrect no implementado.\n");
}

void Game::list(const InstanceId caller, const uint32_t x_coord,
                const uint32_t y_coord) {
    if (!this->characters.count(caller))
        throw Exception("Game::list: unknown caller.");

    Character& character = this->characters.at(caller);

    std::string init_msg;
    std::list<std::string> list_items;

    if (_validateBankerPosition(caller, x_coord, y_coord, false)) {
        BankAccount& account = bank[character.getNickname()];
        account.list(init_msg, list_items);
    } else if (_validatePriestPosition(caller, x_coord, y_coord, false)) {
        _listNPCSellableItems(priest, init_msg, list_items);
    } else {
        Id merchant_id = 0;
        if (!_validateMerchantPosition(caller, merchant_id, x_coord, y_coord,
                                       false)) {
            // devolver mensaje de no hay npc que tenga cosas para listar.
            return;
        }
        _listNPCSellableItems(merchant_id, init_msg, list_items);
    }

    Notification* list = new List(init_msg, list_items);
    this->active_clients.notify(caller, list);
    return;
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

const bool Game::_validateBankerPosition(const InstanceId caller,
                                         const uint32_t x_coord,
                                         const uint32_t y_coord,
                                         const bool reply_if_invalid) {
    Id map_id = this->characters.at(caller).getMapId();

    if (this->map_container[map_id].getTile(x_coord, y_coord).npc == banker)
        return true;

    if (reply_if_invalid) {
        std::string reply_msg = "El NPC seleccionado no es un banquero.";
        Notification* reply = new Reply(ERROR_MSG, reply_msg.c_str());
        active_clients.notify(caller, reply);
    }

    return false;
}

const bool Game::_validatePriestPosition(const InstanceId caller,
                                         const uint32_t x_coord,
                                         const uint32_t y_coord,
                                         const bool reply_if_invalid) {
    Id map_id = this->characters.at(caller).getMapId();

    if (this->map_container[map_id].getTile(x_coord, y_coord).npc == priest)
        return true;

    if (reply_if_invalid) {
        std::string reply_msg = "El NPC seleccionado no es un sacerdote.";
        Notification* reply = new Reply(ERROR_MSG, reply_msg.c_str());
        active_clients.notify(caller, reply);
    }

    return false;
}

const bool Game::_validateMerchantPosition(const InstanceId caller,
                                           Id& merchant_id,
                                           const uint32_t x_coord,
                                           const uint32_t y_coord,
                                           const bool reply_if_invalid) {
    Id map_id = this->characters.at(caller).getMapId();
    Id npc_id = this->map_container[map_id].getTile(x_coord, y_coord).npc;

    for (size_t i = 0; i < merchants.size(); ++i) {
        if (merchants[i] != npc_id)
            continue;

        merchant_id = npc_id;
        return true;
    }

    if (reply_if_invalid) {
        std::string reply_msg = "El NPC seleccionado no es un mercader.";
        Notification* reply = new Reply(ERROR_MSG, reply_msg.c_str());
        active_clients.notify(caller, reply);
    }

    return false;
}

void Game::depositItemOnBank(const InstanceId caller, const uint32_t x_coord,
                             const uint32_t y_coord, const uint8_t n_slot,
                             uint32_t amount) {
    if (!this->characters.count(caller))
        throw Exception("Game::depositItemOnBank: unknown caller.");

    if (!_validateBankerPosition(caller, x_coord, y_coord, true))
        return;

    uint32_t asked_amount = amount;
    Item* to_deposit = nullptr;

    if (!_dropItem(caller, n_slot, amount, &to_deposit))
        return;

    Character& character = this->characters.at(caller);

    BankAccount& account = this->bank[character.getNickname()];

    try {
        account.deposit(to_deposit->getId(), amount);
    } catch (const FullBankAccountException& e) {
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
        return;
    }

    std::string reply_msg = "";

    if (amount < asked_amount)
        reply_msg += "No tenés suficientes items.";

    reply_msg += "Se ha depositado " + to_deposit->what() + " x" +
                 std::to_string(amount) + " en el banco";

    Notification* reply = new Reply(INFO_MSG, reply_msg.c_str());
    active_clients.notify(caller, reply);

    // this->account.debug();
}

void Game::withdrawItemFromBank(const InstanceId caller, const uint32_t x_coord,
                                const uint32_t y_coord, const uint32_t item_id,
                                uint32_t amount) {
    if (!this->characters.count(caller))
        throw Exception("Game::withdrawItemFromBank: unknown caller.");

    if (!_validateBankerPosition(caller, x_coord, y_coord, true))
        return;

    uint32_t asked_amount = amount;
    Item* withdrew = nullptr;

    Character& character = this->characters.at(caller);

    BankAccount& account = this->bank[character.getNickname()];

    try {
        withdrew = account.withdraw(item_id, amount);
    } catch (const InvalidItemIdException& e) {
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
        return;
    }

    try {
        character.takeItem(withdrew, amount);
    } catch (const FullInventoryException& e) {
        account.deposit(withdrew->getId(), amount);
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
        return;
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

    if (!_validateBankerPosition(caller, x_coord, y_coord, true))
        return;

    Character& character = this->characters.at(caller);

    try {
        character.gatherGold(amount);
    } catch (const InsufficientGoldException& e) {
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
        return;
    }

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

    if (!_validateBankerPosition(caller, x_coord, y_coord, true))
        return;

    uint32_t asked_amount = amount;

    Character& character = this->characters.at(caller);

    BankAccount& account = this->bank[character.getNickname()];

    try {
        account.withdrawGold(amount);
    } catch (const NoMoneyAvailableException& e) {
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
        return;
    }

    std::string reply_msg = "";

    try {
        character.takeGold(amount);
    } catch (const GoldMaximumCapacityReachedException& e) {
        account.depositGold(asked_amount - amount);
        reply_msg +=
            "No pudiste extraer todo lo solicitado. No entra más oro en tu "
            "inventario.";
    }

    // agregar mensaje si pide extraer mas de lo que tiene

    reply_msg += "Se ha extraído " + std::to_string(amount) + " oro del banco.";

    Notification* reply = new Reply(INFO_MSG, reply_msg.c_str());
    active_clients.notify(caller, reply);
}

const bool Game::_validateIfNPCSellsItem(const InstanceId caller,
                                         const Id npc_id, const Id item_id) {
    const std::list<Id>& sellable_items = npcs[npc_id].sellable_items;
    std::list<Id>::const_iterator it =
        std::find(sellable_items.begin(), sellable_items.end(), item_id);

    if (it != sellable_items.end())
        return true;

    std::string msg_reply =
        "El item especificado no es válido. [Escriba /listar para ver los "
        "items válidos].";
    Notification* reply = new Reply(ERROR_MSG, msg_reply);
    active_clients.notify(caller, reply);
    return false;
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

    if (_validatePriestPosition(caller, x_coord, y_coord, false))
        npc_id = priest;
    else if (!_validateMerchantPosition(caller, npc_id, x_coord, y_coord,
                                        false)) {
        // En la posición recibida no hay un npc que venda items.
        std::string msg_reply = "No seleccionaste a un NPC que venda items.";
        Notification* reply = new Reply(ERROR_MSG, msg_reply);
        active_clients.notify(caller, reply);
        return;
    }

    if (!_validateIfNPCSellsItem(caller, npc_id, item_id))
        return;

    unsigned int total_price = items[item_id]->getPrice() * amount;

    try {
        character.gatherGold(total_price);
    } catch (const InsufficientGoldException& e) {
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
        return;
    }

    try {
        character.takeItem(this->items[item_id], amount);
    } catch (const std::exception& e) {
        // FullInventoryException, StateCantTakeItemException
        character.takeGold(total_price);
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
        return;
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

    if (_validatePriestPosition(caller, x_coord, y_coord, false))
        npc_id = priest;
    else if (!_validateMerchantPosition(caller, npc_id, x_coord, y_coord,
                                        false)) {
        // En la posición recibida no hay un npc que venda items.
        std::string msg_reply = "No seleccionaste a un NPC que compre items.";
        Notification* reply = new Reply(ERROR_MSG, msg_reply);
        active_clients.notify(caller, reply);
        return;
    }

    // uint32_t asked_amount = amount;
    Item* to_sell = nullptr;

    if (!_dropItem(caller, n_slot, amount, &to_sell))
        return;

    if (!_validateIfNPCSellsItem(caller, npc_id, to_sell->getId())) {
        character.takeItem(to_sell, amount);
        return;
    }

    unsigned int total_price = to_sell->getPrice() * amount;

    try {
        character.takeGold(total_price);
    } catch (const GoldMaximumCapacityReachedException& e) {
        unsigned int amount_to_replenish = amount + to_sell->getPrice();
        if (amount_to_replenish)
            character.takeItem(to_sell, amount_to_replenish);
        Notification* reply = new Reply(ERROR_MSG, e.what());
        active_clients.notify(caller, reply);
    }

    // si amount < asked_amount avisar.

    std::string reply_msg =
        "Has vendido " + std::to_string(amount) + " " + to_sell->what();
    Notification* reply = new Reply(SUCCESS_MSG, reply_msg);
    active_clients.notify(caller, reply);
}

void Game::listConnectedPlayers(const InstanceId caller) {
    fprintf(stderr, "Comando list no implementado.\n");
}

void Game::sendPrivateMessage(const InstanceId caller,
                              const std::string to_nickname,
                              const std::string message) {
    if (!this->characters.count(caller))
        throw Exception("Game::sendPrivateMessage: unknown caller.");

    if (!this->nickname_id_map.count(to_nickname)) {
        std::string reply_msg =
            "No existe jugador con el nickname especificado.";
        Notification* reply = new Reply(ERROR_MSG, reply_msg.c_str());
        active_clients.notify(caller, reply);
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
    if (!this->characters.count(caller))
        throw Exception("Game::sendPrivateMessage: unknown caller.");

    const std::string& caller_nickname =
        this->characters.at(caller).getNickname();

    Notification* notification =
        new Message(caller_nickname, message, GENERAL_MSG);
    this->active_clients.sendMessageToAll(notification, caller);
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
