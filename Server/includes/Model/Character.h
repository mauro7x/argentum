#ifndef __CHARACTER_H__
#define __CHARACTER_H__
//-----------------------------------------------------------------------------
#include <exception>
#include <vector>
//-----------------------------------------------------------------------------
#include "Equipment.h"
#include "Inventory.h"
#include "Item.h"
#include "ItemsContainer.h"
#include "Kind.h"
#include "Level.h"
#include "Position.h"
#include "Race.h"
#include "States.h"
#include "Wearable.h"
//-----------------------------------------------------------------------------
#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/Orientation.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------
#include "config_structs.h"
//-----------------------------------------------------------------------------
#include "../../../Common/includes/DataStructs.h"
//-----------------------------------------------------------------------------

/*
 * Representa al ente manejado por el jugador.
 *
 *  -Se caracteriza mediante una Raza y Clase.
 *  -Tiene vida y mana, y sus respectivos limites.
 *  -Tiene un Estado [Vivo/Muerto], segun el cual
 * puede o no realizar ciertas acciones.
 *  -Tiene un Inventario en el cual almacena Items,
 * ya sea armas, baculos, pociones, o elementos de defensa.
 *  -Tiene un Equipamiento con los elementos de ataque y defensa que
 * porta.
 */
class Character {
   private:
    //-----------------------------------------------------------------------------

    unsigned int health, mana;
    const unsigned int intelligence, constitution, strength, agility;
    unsigned int max_health, max_mana;
    Race race;
    Kind kind;
    State* state;
    Level level;
    Inventory inventory;
    Equipment equipment;
    Position position;

    bool is_moving;
    int moving_cooldown;
    unsigned int attribute_update_time_elapsed;

    int attack_cooldown;

    bool broadcast;

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------

    /*
     * Recibe la cantidad de segundos que pasaron desde la última vez que se
     * llamó.
     *
     * Actualiza health y mana segun el paso del tiempo.
     */
    void _updateTimeDependantAttributes(const unsigned int seconds_elapsed);

    /*
     * Recibe el número de iteraciones que pasaron desde la última vez que
     * se llamó.
     *
     * Actualiza la posición del character según la velocidad de movimiento
     * establecida.
     */
    void _updateMovement(const unsigned int it);

    //-----------------------------------------------------------------------------

   public:
    //-----------------------------------------------------------------------------

    Character(const CharacterCfg& init_data, const RaceCfg& race,
              const KindCfg& kind, MapContainer& map_container,
              const Id init_map, const int init_x_coord, const int init_y_coord,
              ItemsContainer& items_container);
    ~Character();

    Character(const Character&) = delete;
    Character& operator=(const Character&) = delete;
    Character(Character&&) = delete;
    Character& operator=(Character&&) = delete;

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // Actualización de atributos.
    //-----------------------------------------------------------------------------

    /*
     * Recibe la cantidad de iteraciones que ocurrieron desde la última
     * vez que se llamó.
     *
     * Actualiza los atributos del character que se ven afectados por el
     * paso del tiempo:
     *
     * - posición [si el jugador está en movimiento]
     * - vida [si tiene margen de recuperación]
     * - maná [si tiene margen de recuperación]
     */
    void act(const unsigned int it);

    /*
     * Actualiza max_health, max_mana, y los limites de oro de los slots
     * en el inventario.
     *
     * Esta función es llamada cada vez que el character sube de nivel.
     */
    void updateLevelDependantAttributes();

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    //  Movimiento
    //-----------------------------------------------------------------------------

    void startMovingUp();
    void startMovingDown();
    void startMovingRight();
    void startMovingLeft();

    void stopMoving();

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    //  Manejo de items
    //-----------------------------------------------------------------------------

    /*
     * Recibe la posicion del item en el inventario
     * que se quiere equipar, y lo equipa.
     * Si no hay item en dicha posicion, no hace nada.
     */
    void equip(unsigned int n_slot);

    /*
     * Equipa un Wearable al Equipment en su slot, almacenando en el
     * inventario el Wearable anterior.
     *
     * Lanza InvalidInventorySlotNumberException si la posicion
     * especificada es invalida (fuera de rango).
     */
    void equip(Wearable* item);

    /*
     * Recibe la posición del item en el equipamiento
     * que se quiere desequipar.
     *
     * Si no hay item en dicha posición, no hace nada.
     *
     * Lanza FullInventoryException si no hay espacio en
     * el inventario para el elemento desequipado, no pudiendo
     * efectuarse el comando.
     */
    void unequip(unsigned int n_slot);

    /*
     * Toma amount items (del mismo tipo) y los agrega al inventario.
     *
     * Lanza FullInventoryException si el inventario esta lleno
     * y no pudo agregarse.
     */
    void takeItem(Item* item, unsigned int amount = 1);

    /*
     * Dropa la cantidad especificada del item en la n_slot
     * pasada por parámetro.
     *
     * Si la cantidad de items en el slot es menor que amount,
     * se configura en amount la cantidad real droppeada.
     *
     * Si no hay un item en dicha posicion, retorna nullptr.
     *
     * Lanza InvalidInventorySlotNumberException si la posicion
     * especificada es invalida (fuera de rango).
     */
    Item* dropItem(const unsigned int n_slot, unsigned int& amount);

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // Modificación de maná y vida.
    //-----------------------------------------------------------------------------

    /*
     * Efectua la accion curativa de las pociones de mana.
     * Aumenta los puntos de mana en los points especificados,
     * siempre y cuando el limite de mana no se supere.
     */
    void recoverMana(const unsigned int points);

    /*
     * Efectua la accion curativa de las pociones/hechizos de vida.
     * Aumenta los puntos de vida en los points especificados,
     * siempre y cuando el limite de health no se supere.
     */
    void recoverHealth(const unsigned int points);

    /*
     * Si hay suficiente mana, consume mana_points de mana.
     *
     * Lanza InsufficientManaException si no hay suficiente mana.
     */
    void consumeMana(const unsigned int mana_points);

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // Ataque y defensa
    //-----------------------------------------------------------------------------

    /*
     * Metodo llamado al usar el arma equipada.
     *
     * Setea el cooldown de ataque. El arma no podrá volver a utilizarse
     * en tanto no haya pasado el tiempo de cooldown.
     */
    void setAttackCooldown(const unsigned int cooldown);

    /*
     * Metodo llamado al usar báculos.
     *
     * Lanza KindCantDoMagicException si el Kind no puede hacer magia.
     */
    void doMagic();

    /*
     * Verifica si dado el estado actual, el character puede
     * ser atacado.
     *
     * Lanza StateOfCharacterCantBeAttackedException si el estado
     * actual no permite al character ser atacado.
     */
    void beAttacked();

    /*
     * Efectúa la recepción del ataque de otro jugador.
     *
     * Retorna los puntos de daño que efectivamente recibe.
     *
     * Si el ataque es eludible, puede esquivarlo y no recibir daño alguno,
     * por lo que retorna 0.
     *
     * Si no lo puede esquivar, absorbe lo que puede con su defensa, siendo
     * el daño efectivo el causado menos el absorbido por la defensa.
     *
     */
    const unsigned int receiveAttack(const unsigned int damage,
                                     const bool eludible);

    /*
     * Efectua un ataque a otro jugador, usando el arma que tiene equipada.
     *
     * Retorna los puntos de daño efectivos que generó. Si no tiene arma
     * equipada, retorna 0.
     *
     * Si el arma tiene efecto sobre el jugador [e.g: es un baculo curativo],
     * i.e tiene rango cero, se usa inmediatamente.
     *
     * Si el arma es de daño, se verifica si el otro jugador está dentro del
     * rango de dicha arma, y se efectúa el ataque al recibir el atacado los
     * puntos de daño.
     *
     * Lanza:
     *       OutOfRangeAttackException si el otro jugador está fuera del rango
     * del arma.
     *
     *       AttackCooldownTimeNotElapsedException si el cooldown de uso del
     * arma todavía no pasó.
     *
     *       KindCantDoMagicException si la clase del character no puede hacer
     * magia e intenta usar un hechizo.
     * 
     *       CantAttackItselfException si quieren lanzar un ataque dañino sobre
     * sí mismo.
     *
     *       TooHighLevelDifferenceOnAttackException si la diferencia de niveles
     * es más alta de la permitida para un ataque.
     *
     *       NewbiesCantBeAttackedException si el jugador al que se quiere
     * atacar es Newbie.
     *
     *       InsufficientManaException si no puede usar el hechizo debido a
     * déficit de maná.
     *
     *       AttackerStateCantAttackException si el jugador no puede atacar
     * debido a su estado (muerto)
     *
     *       AttackedStateCantBeAttackedException si el jugador al que se
     * quiere atacar no puede atacar debido a su estado (muerto).
     */
    const int attack(Character& attacked);

    /*
     * Cuando health es cero, se llama a este método. Cambia el estado del
     * jugador a Dead.
     */
    void die();

    /*
     * Recibe un vector en el que dropeará todos sus elementos,
     * tanto en equipment como en inventory.
     */
    void dropAllItems(std::vector<DroppingSlot>& dropped_items);

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // Obtención de estado
    //-----------------------------------------------------------------------------

    /* Retorna si el character es newbie o no. */
    const bool isNewbie() const;

    /* Retorna la posición del character. */
    const Position& getPosition() const;

    /* Retorna el nivel del character. */
    const unsigned int getLevel() const;

    /* Retorna la health del character. */
    const unsigned int getHealth() const;

    /* Retorna el health máxima del character. */
    const unsigned int getMaxHealth() const;

    /* Retorna el id del mapa en el que se encuentra */
    const Id getMapId() const;

    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    // Broadcast control
    //-----------------------------------------------------------------------------

    /*
     * Retorna si el character debe ser broadcasteado
     * [sufrió modificaciones] o no.
     */
    const bool mustBeBroadcasted() const;

    /*
     * Es notificado que se lo ha broadcasteado.
     */
    void beBroadcasted();

    /*
     * Recibe una estructura de broadcast de tipo PlayerData,
     * y la llena con sus atributos actuales para su broadcasteo.
     *
     * [No llena los campos: id, nickname]
     */
    void fillBroadcastData(PlayerData& data) const;

    //-----------------------------------------------------------------------------

    void debug();
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class InsufficientManaException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

class CantAttackInSafeZoneException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

class CantAttackItselfException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

class OutOfRangeAttackException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

class NewbiesCantBeAttackedException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

class AttackCooldownTimeNotElapsedException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

class TooHighLevelDifferenceOnAttackException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

class KindCantDoMagicException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
