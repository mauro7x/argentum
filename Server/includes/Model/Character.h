#ifndef __CHARACTER_H__
#define __CHARACTER_H__
//-----------------------------------------------------------------------------
#include <exception>
//-----------------------------------------------------------------------------
#include "Kind.h"
#include "Race.h"
#include "States.h"
#include "Inventory.h"
#include "Equipment.h"
#include "Level.h"
#include "Item.h"
#include "Wearable.h"
#include "Position.h"
//-----------------------------------------------------------------------------
#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/Orientation.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------
#include "config_structs.h"
//-----------------------------------------------------------------------------
#include "../../../Common/includes/UnitData.h"
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

        Orientation moving_orientation;
        bool moving;
        int moving_cooldown_time;
        unsigned int attribute_update_time_elapsed;

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

        Character(const CharacterCfg& init_data,
                  const RaceCfg& race, const KindCfg& kind,
                  MapContainer& map_container,
                  const Id init_map,
                  const int init_x_coord,
                  const int init_y_coord);
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
        void equip(unsigned int inventory_position);

        /*
         * Equipa un Wearable al Equipment en su slot, almacenando en el
         * inventario el Wearable anterior.
         */
        void equip(Wearable* item);

        /*
         * Toma un item y lo agrega al inventario.
         * Retorna la posicion en la que se agrego el item.
         * 
         * Lanza FullInventoryException si el inventario esta lleno
         * y no pudo agregarse.
         */
        const unsigned int takeItem(Item* item);

        /*
         * Devuelve el item en la inventory_position especificada.
         * Si no hay un item en dicha posicion, retorna nullptr.
         * 
         * Lanza InvalidPositionException si la posicion
         * especificada es invalida (fuera de rango).
         */
        Item* dropItem(const unsigned int inventory_position);

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
         * Retorna los puntos de daño efectivos que generó. Si no tiene arma equipada,
         * o el arma es curativa, retorna 0.
         * 
         * Si el arma tiene efecto sobre el jugador [e.g: es un baculo curativo],
         * i.e tiene rango cero, se usa inmediatamente.
         * 
         * Si el arma es de daño, se verifica si el otro jugador está dentro del
         * rango de dicha arma, y se efectúa el ataque al recibir el atacado los puntos
         * de daño.
         * 
         * Lanza:
         *       OutOfRangeAttackException si el otro jugador está fuera del rango del arma.
         * 
         *       KindCantDoMagicException si la clase del character no puede hacer magia
         * e intenta usar un hechizo.
         * 
         *       TooHighLevelDifferenceOnAttackException si la diferencia de niveles es más
         * alta de la permitida para un ataque.
         * 
         *       NewbiesCantBeAttackedException si el jugador al que se quiere atacar es Newbie.
         * 
         *       InsufficientManaException si no puede usar el hechizo debido a déficit de maná.
         * 
         *       ActualStateCantBeAttackedException si el jugador al que se quiere atacar
         * tiene un estado en el que no puede ser atacado.
         */
        const unsigned int attack(Character& attacked);

        /*
         * Cuando health es cero, se llama a este método. Cambia el estado del jugador
         * a Dead.
         */
        void die();

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
         * 
         * ??? EL ID DEL MAPA ???
         */
        void fillBroadcastData(PlayerData& data) const;

        //-----------------------------------------------------------------------------

        void debug();
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class InsufficientManaException: std::exception {
    public:
        virtual const char* what() const noexcept;
};

class OutOfRangeAttackException: std::exception {
    public:
        virtual const char* what() const noexcept;
};

class NewbiesCantBeAttackedException: std::exception {
    public:
        virtual const char* what() const noexcept;
};

class TooHighLevelDifferenceOnAttackException: std::exception {
    public:
        virtual const char* what() const noexcept;
};

class ActualStateCantBeAttackedException: std::exception {
    public:
        virtual const char* what() const noexcept;
};

class KindCantDoMagicException: std::exception {
    public:
        virtual const char* what() const noexcept;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
