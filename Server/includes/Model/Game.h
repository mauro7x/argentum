#ifndef __GAME_H__
#define __GAME_H__
//-----------------------------------------------------------------------------
#include <cstdint>
#include <unordered_map>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------
#include "../../includes/Control/Notification.h"
#include "../../includes/Control/Database.h"
//-----------------------------------------------------------------------------
#include "Character.h"
#include "Config.h"
#include "Creature.h"
#include "ItemsContainer.h"
#include "config_structs.h"
//-----------------------------------------------------------------------------

// Forward declaration
class ActiveClients;

struct MapCreaturesInfo {
    MapCreaturesInfo(unsigned int amount_of_creatures,
                     int creature_spawning_cooldown);
    unsigned int amount_of_creatures;
    int creature_spawning_cooldown;
};

class Game {
   private:
    //--------------------------------------------------------------------------
    // Game components configuration files:
    //--------------------------------------------------------------------------
    Config<RaceCfg> races;
    Config<KindCfg> kinds;
    Config<CreatureCfg> creatures_data;
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Game entities
    //--------------------------------------------------------------------------
    MapContainer map_container;
    ItemsContainer items;
    std::unordered_map<InstanceId, Character> characters;
    std::unordered_map<InstanceId, Creature> creatures;
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Additional attributes
    //--------------------------------------------------------------------------
    InstanceId next_instance_id;
    ActiveClients& active_clients;
    std::unordered_map<Id, MapCreaturesInfo> maps_creatures_info;
    int creature_spawn_cooldown;
    //--------------------------------------------------------------------------

   public:
    //--------------------------------------------------------------------------
    Game(ActiveClients& active_clients);
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&& other) = delete;
    Game& operator=(Game&& other) = delete;
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
     * Ante actualizaciones (NEW, UPDATE, DELETE) de un jugador, 
     * se invoca este método. Su función es broadcastear dicha actualización,
     * pusheándola a los clientes activos.
     * 
     * El atributo send_to_caller indica si dicho broadcast debe ser
     * enviado al jugador que se actualizo o no.
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
     * Ante la conexión de un nuevo jugador o el cambio de mapa, se invoca
     * este método para enviarle a dicho jugador un full broadcast con
     * toda la información del juego. 
     */
    void _pushFullBroadcast(InstanceId receiver, bool is_new_connection);

    /* método provisorio */
    void broadcastNewCharacter(InstanceId id);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Creación y eliminación de entidades
    //--------------------------------------------------------------------------

    // DEFINIR COMO VIENE EL PLAYERDATA SI ES NUEVO!.
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

    /* 
     * Escoge aleatoriamente una criatura entre las disponibles en la config,
     * y deuvelve su Id.
     */
    const Id _randomSelectCreature() const;

    /*
     * Recibe un mapa en el cual spawnea una criatura aleatoria.
     */
    void _spawnNewCreature(const Id spawning_map);

    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Actualización del loop
    //--------------------------------------------------------------------------

    void actCharacters(const int it);

    void actCreatures(const int it);

    /*
     * Spawnea criaturas en cada mapa según TIME_TO_SPAWN_CREATURE, hasta
     * alcanzar el número máximo MAX_CREATURES_PER_MAP.
     */
    void spawnNewCreatures(const int it);
    
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    // Comandos
    //--------------------------------------------------------------------------
    void startMovingUp(const InstanceId caller);
    void startMovingDown(const InstanceId caller);
    void startMovingLeft(const InstanceId caller);
    void startMovingRight(const InstanceId caller);

    void stopMoving(const InstanceId caller);

    void useWeapon(const InstanceId caller, const uint32_t x_coord,
                   const uint32_t y_coord);

    void equip(const InstanceId caller, const uint8_t n_slot);

    void meditate(const InstanceId caller);

    void resurrect(const InstanceId caller);

    void list(const InstanceId caller, const uint32_t x_coord, const uint32_t y_coord);

    void depositItemOnBank(const InstanceId caller, const uint32_t x_coord,
                           const uint32_t y_coord, const uint8_t n_slot,
                           uint32_t amount);
    void withdrawItemFromBank(const InstanceId caller, const uint32_t x_coord,
                              const uint32_t y_coord, const uint32_t item_id,
                              const uint32_t amount);

    void depositGoldOnBank(const InstanceId caller, const uint32_t x_coord,
                           const uint32_t y_coord, const uint32_t amount);
    void withdrawGoldFromBank(const InstanceId caller, const uint32_t x_coord,
                              const uint32_t y_coord, const uint32_t amount);

    void buyItem(const InstanceId caller, const uint32_t x_coord,
                 const uint32_t y_coord, const uint32_t item_id,
                 const uint32_t amount);
    void sellItem(const InstanceId caller, const uint32_t x_coord,
                  const uint32_t y_coord, const uint8_t n_slot,
                  const uint32_t amount);

    void take(const InstanceId caller);
    void drop(const InstanceId caller, const uint8_t n_slot, const uint32_t amount);

    void listConnectedPlayers(const InstanceId caller);

    //-------------------------------------------------------------------------

    const Id getMapId(const InstanceId caller);
};
//-----------------------------------------------------------------------------
#endif  // __GAME_H__
