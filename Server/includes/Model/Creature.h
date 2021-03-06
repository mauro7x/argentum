#ifndef __CREATURE_H__
#define __CREATURE_H__
//-----------------------------------------------------------------------------
#include <exception>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/DataStructs.h"
#include "../../../Common/includes/Orientation.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------
#include "Attackable.h"
#include "Character.h"
#include "Formulas.h"
#include "ItemsContainer.h"
#include "Position.h"
#include "config_structs.h"
#include "LogicMaps.h"
//-----------------------------------------------------------------------------
class Game;
//-----------------------------------------------------------------------------

class Creature : public Attackable {
   private:
    Id id;
    std::string name;
    uint32_t health_max;
    uint32_t health;
    uint32_t min_damage;
    uint32_t max_damage;
    Position position;
    uint32_t visible_range;
    int movement_speed;

    ItemsContainer& items;
    LogicMaps& logic_maps;
    Id map_id;
    std::unordered_map<InstanceId, Character>& characters;
    std::array<Orientation, N_ORIENTATIONS> posibles_orientations;
    Game& game;

    int moving_cooldown;
    int random_moving_cooldown;
    unsigned int attribute_update_time_elapsed;
    int attack_cooldown;
    int actual_attack_cooldown;
    InstanceId attacking_id;
    unsigned int level;
    bool broadcast;

    Formulas& formulas;

    // Configuración
    const int& rate;
    const unsigned int random_movement_factor;

    /*
     * Recorrer todo el map de characteres, si encuentra characteres dentro del
     * rango visible, si no devuelve 0
     */
    InstanceId _getNearestCharacter();

    /*
     * Verificar si el character esta suficiente cerca para atacar.
     */
    bool _attackNearstCharacter(const Position& position_character);

    /*
     * Determinar la direccion mas lejo y empieza a mover.
     */
    void _determinateDirectionAndMove(const Position& position_character);

    /*
     * Hacer el damage al character mas cerca si esta listo a pegar
     */

    void _updateDamage(const unsigned int it, const InstanceId id);
    /*
     * Recibe el número de iteraciones que pasaron desde la última vez que
     * se llamó.
     *
     * Actualiza la posición del character según la velocidad de movimiento
     * establecida.
     */
    void _updateMovement(const unsigned int it);

    /*
     * Setter una direccion aleatoria a mover cuando no hay jugador cercano;
     */
    void _setRandomOrientation();
    /*
     * Recibe el número de iteraciones que pasaron desde la última vez que
     * se llamó.
     *
     *
     * Genere una direccion valida random y actualiza la posición del character
     * según la velocidad de movimiento establecida.
     */
    void _randomMovement(const unsigned int it);

   public:
    //-----------------------------------------------------------------------------

    /* Constructor */
    Creature(const CreatureCfg& data, LogicMaps& logic_maps,
             const Id init_map, const int init_x_coord, const int init_y_coord,
             const uint32_t health, ItemsContainer& items,
             std::unordered_map<InstanceId, Character>& characters, Game& game,
             Formulas& formulas, const int& rate,
             const unsigned int random_movement_factor);

    /* Destructor */
    ~Creature();

    /* Deshabilitamos el constructor por copia. */
    Creature(const Creature&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Creature& operator=(const Creature&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Creature(Creature&&) = delete;

    /* Deshabilitamos el operador= para movimiento. */
    Creature& operator=(Creature&&) = delete;

    /*
     * Recibe la cantidad de iteraciones que ocurrieron desde la última
     * vez que se llamó.
     *
     * Actualiza los atributos de la creatura que se ven afectados por el
     * paso del tiempo:
     *
     * - posición [si la criatura está en movimiento]
     */
    void act(const unsigned int it);

    /*
     * Efectúa la recepción del ataque de un character.
     */
    const bool receiveAttack(int& damage, const bool eludible) override;

    /* Retorna la posición de la criatura */
    const Position& getPosition() const override;

    /* Lanza CantRecoverCreaturesHealthException */
    const bool recoverHealth(int& points) override;

    /*
     * Si muere la criatura, se llama a este método.
     *
     * Dropea algo en el slot según la función de probabilidad:
     * ----------------------------------------------------------------------
     * |probabilidad | item droppeado                                        |
     * |-------------|-------------------------------------------------------|
     * |   0.80      | Nada                                                  |
     * |   0.16      | Oro, una cantidad igual a rand(0.01, 0.2) * VidaMaxNPC|
     * |   0.02      | Un poción de vida o mana elegida al azar              |
     * |   0.02      | Cualquier otro objeto al azar                         |
     * ----------------------------------------------------------------------
     */
    void dropAllItems(std::vector<DroppingSlot>& dropped_items) override;

    /* Llena la información de broadcast de la criatura */
    void fillBroadcastData(CreatureData& data) const;

    /* Devuelve si la criatura debe ser broadcasteada */
    const bool mustBeBroadcasted() const;

    /* Avisa que la criatura es broadcasteada */
    void beBroadcasted();

    /* Devuelve true. Las criaturas siempre pueden ser atacadas. */
    const bool beAttacked();

    /* Devuelve false. Las criaturas no califican como Newbie. */
    const bool isNewbie() const;

    /* Retorna la vida máxima de la criatura */
    const unsigned int getMaxHealth() const override;

    /* Retorna la vida actual de la criatura */
    const unsigned int getHealth() const override;

    /* Retorna el nivel de la criatura */
    const unsigned int getLevel() const;

    /* Retorna la id del mapa en el que se encuentra la criatura */
    const Id getMapId() const;

    /* Retorna true */
    const bool isCreature() const override;

    /* Retorno la id del character que esta atacando*/
    const InstanceId getAttackingCharacterId() const;
};

class CantRecoverCreaturesHealthException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

#endif  //__CREATURE_H__
