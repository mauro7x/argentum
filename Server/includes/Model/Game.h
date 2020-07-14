#ifndef __GAME_H__
#define __GAME_H__

//-----------------------------------------------------------------------------
#include <cstdint>
#include <string>
#include <unordered_map>
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../../Common/includes/JSON.h"
#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "../../includes/Control/Database.h"
#include "../../includes/Control/Notifications/Notification.h"
#include "../defs.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
#include "Bank.h"
#include "BankAccount.h"
#include "Character.h"
#include "Config.h"
#include "Creature.h"
#include "Formulas.h"
#include "ItemsContainer.h"
#include "config_structs.h"
//-----------------------------------------------------------------------------

// Forward declaration
class ActiveClients;

//-----------------------------------------------------------------------------
// Estructuras auxiliares
//-----------------------------------------------------------------------------

struct GameCfg {
    unsigned int critical_attack_dmg_modifier;
    unsigned int random_movement_factor;
    unsigned int max_creatures_per_map;
    unsigned int ms_to_update_character_attributes;
    unsigned int ms_to_spawn_creature;
    unsigned int ms_to_disappear_dropped_item;
    unsigned int ms_to_persist_data;
};

//-----------------------------------------------------------------------------

struct MapCreaturesInfo {
    MapCreaturesInfo(unsigned int amount_of_creatures,
                     int creature_spawning_cooldown);
    unsigned int amount_of_creatures;
    int creature_spawning_cooldown;
};

//-----------------------------------------------------------------------------

struct ResurrectionInfo {
    ResurrectionInfo(int cooldown, int priest_x_coord, int priest_y_coord);
    int cooldown;
    int time_since_last_message;
    int priest_x_coord;
    int priest_y_coord;
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class Game {
   private:
    //--------------------------------------------------------------------------
    // Config
    //--------------------------------------------------------------------------
    const int& rate;
    GameCfg cfg = {};
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Game components configuration files:
    //--------------------------------------------------------------------------
    Config<RaceCfg> races;
    Config<KindCfg> kinds;
    Config<CreatureCfg> creatures_data;
    Config<NPCCfg> npcs;
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Game entities
    //--------------------------------------------------------------------------
    MapContainer map_container;
    ItemsContainer items;

    std::unordered_map<InstanceId, Character> characters;
    std::unordered_map<InstanceId, Creature> creatures;

    Bank bank;
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Cooldowns
    //--------------------------------------------------------------------------
    std::unordered_map<Id, MapCreaturesInfo> maps_creatures_info;

    // Las claves de los siguientes mapas de cooldown de items droppeados
    // se obtienen a partir de las coordenadas
    // del tile que contiene el item droppeado.
    // e.g: (x = 1, y = 2)   ---> key = "1,2"
    //      (x = 2, y = 1)   ---> key = "2,1"
    //      (x = 39, y = 27) ---> key = "39,27"
    std::unordered_map<Id, std::unordered_map<std::string, int>>
        dropped_items_lifetime_per_map;

    std::unordered_map<Id, ResurrectionInfo> resurrecting_players_cooldown;

    int data_persistence_cooldown;
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Additional attributes
    //--------------------------------------------------------------------------
    InstanceId next_instance_id;
    ActiveClients& active_clients;
    std::unordered_map<std::string, InstanceId> nickname_id_map;

    Id banker;
    Id priest;
    std::vector<Id> merchants;

    std::unordered_map<Id, std::vector<std::string>> priests_position;

    Database& database;
    Formulas formulas;
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Métodos de carga de configuración
    //--------------------------------------------------------------------------

    /* Carga la configuración del json correspondiente */
    void _loadCfg();

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Métodos auxiliares de creación del game
    //--------------------------------------------------------------------------

    /*
     * Carga en el mapa priests_position las posiciones de los sacerdotes en
     * los distintos mapas.
     */
    void _establishPriestsPosition(std::vector<Id>& maps_id);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Métodos auxiliares de creación de characters
    //--------------------------------------------------------------------------

    /*
     * Establece la posición de spawneo del character.
     *
     * Si el character fue recién creado, (new_created = true), se asigna una
     * posición aleatoria en una zona segura del mapa inicial dictaminado por la
     * configuración recibida.
     *
     * Si el character ya estaba creado, se spawnea en la posición persistida o
     * algún tile adyacente a ella (si está ocupada). Si no se puede encontrar
     * posición libre a su alrededor, se spawneará como si fuera recién creada.
     */
    void _establishCharacterSpawningPosition(const CharacterCfg& init_data,
                                             const Id& spawning_map_id,
                                             int& spawning_x_coord,
                                             int& spawning_y_coord);

    /*
     * Llena la cuenta del banco del jugador con los datos persistidos.
     */
    void _loadBankAccount(const CharacterCfg& init_data);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Métodos auxiliares para creación de creatures
    //--------------------------------------------------------------------------

    /*
     * Escoge aleatoriamente una criatura entre las disponibles en la config,
     * y deuvelve su Id.
     */
    const Id _randomSelectCreature(const Id spawning_map) const;

    /*
     * Recibe un mapa en el cual spawnea una criatura aleatoria.
     */
    void _spawnNewCreature(const Id spawning_map);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Métodos de broadcast
    //--------------------------------------------------------------------------

    /*
     * Construye el broadcast del character cuya id es recibida.
     *
     * Alloca memoria, que luego deberá ser desallocada por el caller.
     */
    Notification* _buildPlayerBroadcast(InstanceId id,
                                        BroadcastType broadcast_type);

    /*
     * Construye el broadcast de la creature cuya id es recibida.
     *
     * Alloca memoria, que luego deberá ser desallocada por el caller.
     */
    Notification* _buildCreatureBroadcast(InstanceId id,
                                          BroadcastType broadcast_type);

    /*
     * Construye el broadcast del item droppeado en el mapa cuyas coordenadas
     * e id de mapa son recibidas.
     *
     * Alloca memoria, que luego deberá ser desallocada por el caller.
     */
    Notification* _buildItemBroadcast(Id map_id, int x_coord, int y_coord,
                                      BroadcastType broadcast_type);

    /*
     * Ante actualizaciones (NEW, UPDATE, DELETE) de un jugador,
     * se invoca este método. Su función es broadcastear dicha
     * actualización, pusheándola a los clientes activos.
     *
     * El atributo send_to_caller indica si dicho broadcast debe ser
     * enviado al jugador que se actualizó o no.
     */
    void _pushCharacterDifferentialBroadcast(InstanceId id, BroadcastType type,
                                             bool send_to_caller);

    /*
     * Ante actualizaciones (NEW, UPDATE, DELETE) de una criatura,
     * se invoca este método. Su función es broadcastear dicha actualización,
     * pusheándola a los clientes activos.
     */
    void _pushCreatureDifferentialBroadcast(InstanceId creature,
                                            BroadcastType broadcast_type);

    /*
     * Ante actualizaciones (NEW, DELETE) de un item droppeado en el mapa,
     * se invoca este método. Su función es broadcastear dicha actualización,
     * pusheándola a los clientes activos.
     */
    void _pushItemDifferentialBroadcast(Id map_id, int x_coord, int y_coord,
                                        BroadcastType broadcast_type);

    /*
     * Envían un evento que desenvuelve efectos multimedia que afectan a todos
     * los jugadores.
     */
    void _pushCharacterEvent(InstanceId character_id, EventType type);
    void _pushCreatureEvent(InstanceId creature_id, EventType type);

    /*
     * Envía un evento que desenvuelve efectos multimedia que afecta sólo al
     * jugador.
     */
    void _pushCharacterMainEvent(InstanceId character_id, EventType type);

    /*
     * Ante la conexión de un nuevo jugador o el cambio de mapa, se invoca
     * este método para enviarle a dicho jugador un full broadcast con
     * toda la información del juego.
     *
     * Dicho full broadcast contiene toda la información de los characters,
     * creatures e items en el juego.
     */
    void _pushFullBroadcast(InstanceId receiver, bool is_new_connection);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Métodos auxiliares de persistencia
    //--------------------------------------------------------------------------

    /*
     * Persiste la información de todos los jugadores conectados.
     */
    void _persistAllData(Database& database);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Validación de presencia de entes según la posición
    //--------------------------------------------------------------------------

    /*
     * Retorna true si en la posición que corresponde a las coordenadas
     * recibidas se encuentra un banquero, false en caso contrario.
     *
     * Si exception_if_invalid es true, se lanza una Exception con un mensaje
     * indicando la incongruencia.
     */
    const bool _validateBankerPosition(const InstanceId caller, Id& npc_id,
                                       const uint32_t x_coord,
                                       const uint32_t y_coord,
                                       const bool exception_if_invalid);
    /*
     * Retorna true si en la posición que corresponde a las coordenadas
     * recibidas se encuentra un sacerdote, false en caso contrario.
     *
     * Si exception_if_invalid es true, se lanza una Exception con un mensaje
     * indicando la incongruencia.
     */
    const bool _validatePriestPosition(const InstanceId caller, Id& npc_id,
                                       const uint32_t x_coord,
                                       const uint32_t y_coord,
                                       const bool exception_if_invalid);
    /*
     * Retorna true si en la posición que corresponde a las coordenadas
     * recibidas se encuentra un mercader, false en caso contrario.
     *
     * Si exception_if_invalid es true, se lanza una Exception con un mensaje
     * indicando la incongruencia.
     */
    const bool _validateMerchantPosition(const InstanceId caller, Id& npc_id,
                                         const uint32_t x_coord,
                                         const uint32_t y_coord,
                                         const bool exception_if_invalid);
    /*
     * Retorna true si en la posición que corresponde a las coordenadas
     * recibidas se encuentra un portal, false en caso contrario.
     *
     * Si exception_if_invalid es true, se lanza una Exception con un mensaje
     * indicando la incongruencia.
     */
    const bool _validatePortalPosition(const InstanceId caller,
                                       const uint32_t x_coord,
                                       const uint32_t y_coord,
                                       const bool exception_if_invalid);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Métodos auxiliares para la ejecución de los comandos
    //--------------------------------------------------------------------------

    /*
     * Valida si el map_id especificado corresponde a un mapa al cual un
     * portal puede teletransportar.
     *
     * Lanza Exception si no.
     */
    void _validatePortalMapId(const InstanceId caller, Id map_id);

    /*
     * Valida si el item_id especificado corresponde a un item que el NPC puede
     * comercializar.
     *
     * Lanza Exception si no.
     */
    void _validateIfNPCMarketsItem(const InstanceId caller, const Id npc_id,
                                   const Id item_id);

    /*
     * Lista los mapas posibles a los cuales un portal puede teletransportar.
     * Además, establece un mensaje inicial descriptivo en init_msg.
     */
    void _listPortalMaps(std::string& init_msg,
                         std::list<std::string>& item_list);

    /*
     * Lista los items que el NPC puede comercializar, con su descripción.
     * Además, establece un mensaje inicial descriptivo en init_msg.
     */
    void _listNPCSellableItems(const Id npc_id, std::string& init_msg,
                               std::list<std::string>& item_list);

    /*
     * Efectúa el dropeo del item en el n_slot especificado.
     *
     * Devuelve si se dropeó algún item (true) o no (false).
     *
     * Propaga InvalidInventorySlotNumberException.
     */
    const bool _dropItem(const InstanceId caller, const uint8_t n_slot,
                         uint32_t& amount, Item** dropped);

    /*
     * Este método se llama ante la muerte de un character.
     *
     * Efectúa el dropeo de todos los items presentes en el inventario y
     * equipamiento de un character, y los dispone todos los que pueda en el
     * mapa. Si por alguna razón no se pueden disponer todos los items en el
     * mapa, simplemente termina su ejecución.
     */
    void _dropAllItems(Attackable* dropper);

    /*
     * Efectúa el uso de un arma por parte de un character, y envía las
     * notificaciones pertinentes según el resultado de dicho uso.
     *
     * Propaga las siguientes excepciones:
     *  OutOfRangeAttackException,
     *  CantAttackWithoutWeaponException,
     *  KindCantDoMagicException,
     *  TooHighLevelDifferenceOnAttackException,
     *  NewbiesCantBeAttackedException, InsufficientManaException,
     *  AttackedActualStateCantBeAttackedException,
     *  AttackCooldownTimeNotElapsedException,
     *  CantAttackItselfException
     */
    void _useWeapon(const InstanceId caller, const InstanceId target,
                    Attackable* attacked, const bool target_is_creature);

    /*
     * Este método es llamado una vez termina el cooldown de resurrección de un
     * character.
     *
     * Efectúa la resurrección a distancia de un jugador. Esto implica
     * teletransportar al jugador hacia el sacerdote más cercano (si se puede),
     * resucitarlo, y hacer las notificaciones pertinentes.
     */
    void _cooldownResurrect(const InstanceId player);

    /*
     * Envía los mensajes y eventos pertinentes según el resultado del ataque
     * por parte de un character.
     */
    void _sendCharacterAttackNotifications(const int damage, const bool eluded,
                                           const InstanceId caller,
                                           const InstanceId target);

    /*
     * Envía los mensajes y eventos pertinentes según el resultado del ataque
     * por parte de una creature.
     */
    void _sendCreatureAttackNotifications(const int damage,
                                          const InstanceId caller,
                                          const InstanceId target);

    /*
     * Envía los mensajes y eventos pertinentes según el resultado de que un
     * character sea atacado por una creature.
     */
    void _sendAttackedByCreatureNotifications(const int damage,
                                              const bool eluded,
                                              const InstanceId caller);

    /*
     * Envía los mensajes y eventos pertinentes tras una resurrección.
     */
    void _notifyResurrection(const InstanceId caller);

    //--------------------------------------------------------------------------

   public:
    //--------------------------------------------------------------------------

    Game(ActiveClients& active_clients, const int& rate, Database& Database);
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&& other) = delete;
    Game& operator=(Game&& other) = delete;

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Creación y eliminación de entidades
    //--------------------------------------------------------------------------

    /*
     * Recibe un struct CharacterCfg con toda la información persistida
     * del character, o bien la información necesaria para crear un nuevo
     * character.
     *
     * Retorna el id único de instancia de dicho character, mediante el cual
     * se interactuará con el mismo.
     *
     * Lanza Exception si alguno de los id no mapea a ninguna raza/clase.
     */
    const InstanceId newCharacter(const CharacterCfg& init_data);

    /* Una vez se agrega el cliente a active clients, se llama a este método. */
    void broadcastNewCharacter(InstanceId id);

    /*
     * Recibe un struct CreatureCfg con toda la información necesaria
     * para crear una criatura en el mapa especificado.
     *
     * Lanza Exception si el id no mapea a ninguna config de criatura,
     * o el id del mapa inicial no mapea a ningun mapa disponible.
     */
    void newCreature(const CreatureCfg& init_data, const Id init_map);

    /*
     * Llamar a este metodo ante la desconexión de un character.
     *
     * Recibe el id de instancia del character a eliminar.
     * Lo persiste, y luego lo elimina del juego.
     *
     * Lanza Exception si el id especificado no corresponde a ningún
     * character en el juego.
     */
    void deleteCharacter(const InstanceId id, Database& database);

    /*
     * Llamar a este método ante la muerte de una criatura.
     *
     * Recibe el id de instancia de la criatura a eliminar.
     *
     * Efectúa el drop aleatorio de elementos.
     *
     * Lanza Exception si el id especificado no corresponde a ninguna
     * criatura del juego.
     */
    void deleteCreature(const InstanceId id);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Actualización del loop
    //--------------------------------------------------------------------------

    void actCharacters(const int it);

    void actCreatures(const int it);

    /*
     * Spawnea criaturas en cada mapa según el tiempo de spawneo, hasta
     * alcanzar el número máximo.
     */
    void spawnNewCreatures(const int it);

    /*
     * Guarda periódicamente los datos del jugador según TIME_TO_SAVE_DATA.
     */
    void persistPeriodicData(Database& database, const int it);

    /*
     * Actualiza el tiempo de vida de los items droppeados en el mapa,
     * y si alguno de ellos llega a cero los elimina del mapa.
     */
    void updateDroppedItemsLifetime(const int it);

    /*
     * Actualiza el cooldown de los jugadores que están resucitando, y si alguno
     * de ellos llega a cero los resucita, se transporta hacia el sacerdote
     * resurrector y es notificado.
     */
    void updateResurrectingPlayersCooldown(const int it);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Comandos
    //--------------------------------------------------------------------------
    void startMovingUp(const InstanceId caller);
    void startMovingDown(const InstanceId caller);
    void startMovingLeft(const InstanceId caller);
    void startMovingRight(const InstanceId caller);

    void stopMoving(const InstanceId caller);

    void useWeapon(const InstanceId caller, const InstanceId target);

    void equip(const InstanceId caller, const uint8_t n_slot);
    void unequip(const InstanceId caller, const uint8_t n_slot);

    void meditate(const InstanceId caller);

    void resurrect(const InstanceId caller);

    void resurrect(const InstanceId caller, const uint32_t x_coord,
                   const uint32_t y_coord);

    void heal(const InstanceId caller, const uint32_t x_coord,
              const uint32_t y_coord);

    void list(const InstanceId caller, const uint32_t x_coord,
              const uint32_t y_coord);

    void depositItemOnBank(const InstanceId caller, const uint32_t x_coord,
                           const uint32_t y_coord, const uint8_t n_slot,
                           uint32_t amount);
    void withdrawItemFromBank(const InstanceId caller, const uint32_t x_coord,
                              const uint32_t y_coord, const uint32_t item_id,
                              uint32_t amount);

    void depositGoldOnBank(const InstanceId caller, const uint32_t x_coord,
                           const uint32_t y_coord, const uint32_t amount);
    void withdrawGoldFromBank(const InstanceId caller, const uint32_t x_coord,
                              const uint32_t y_coord, uint32_t amount);

    void buyItem(const InstanceId caller, const uint32_t x_coord,
                 const uint32_t y_coord, const uint32_t item_id,
                 const uint32_t amount);
    void sellItem(const InstanceId caller, const uint32_t x_coord,
                  const uint32_t y_coord, const uint8_t n_slot,
                  uint32_t amount);

    void take(const InstanceId caller);

    void drop(const InstanceId caller, const uint8_t n_slot, uint32_t amount);

    void listConnectedPlayers(const InstanceId caller);

    void sendPrivateMessage(const InstanceId caller,
                            const std::string to_nickname,
                            const std::string message);

    void sendGeneralMessage(const InstanceId caller, const std::string message);

    void help(const InstanceId caller, const uint32_t x_coord,
              const uint32_t y_coord);

    void teleport(const InstanceId caller, const uint32_t portal_x_coord,
                  const uint32_t portal_y_coord, const Id map_id);

    //-------------------------------------------------------------------------

    const Id getMapId(const InstanceId caller);

    //-------------------------------------------------------------------------

    void attackedByCreature(const InstanceId caller, int& damage, bool eluded);
};

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Funciones auxiliares
//-----------------------------------------------------------------------------

const std::string _coordinatesToMapKey(int x, int y);
void _mapKeyToCoordinates(const std::string& key, int& x, int& y);

//-----------------------------------------------------------------------------
#endif  // __GAME_H__
//-----------------------------------------------------------------------------
