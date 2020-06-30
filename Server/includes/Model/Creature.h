#ifndef __CREATURE_H__
#define __CREATURE_H__
//-----------------------------------------------------------------------------
#include <exception>
//-----------------------------------------------------------------------------
#include "../../../Common/includes/DataStructs.h"
#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/Orientation.h"
#include "../../../Common/includes/types.h"
//-----------------------------------------------------------------------------
#include "Attackable.h"
#include "Formulas.h"
#include "ItemsContainer.h"
#include "Position.h"
#include "config_structs.h"
//-----------------------------------------------------------------------------
#define DROP_NOTHING_PROB 0.70
#define DROP_GOLD_PROB 0.10
#define DROP_POTION_PROB 0.10
#define DROP_ITEM_PROB 0.10
//-----------------------------------------------------------------------------

class Creature : public Attackable {
   private:
    Id id;
    std::string name;
    uint32_t health_max;
    uint32_t health;
    uint32_t damage;
    Position position;

    ItemsContainer& items;

    Orientation moving_orientation;
    bool moving;
    unsigned int moving_time_elapsed;
    unsigned int attribute_update_time_elapsed;

    bool broadcast;

   public:
    //-----------------------------------------------------------------------------

    /* Constructor */
    Creature(const CreatureCfg& data, MapContainer& map_container,
             const Id init_map, const int init_x_coord, const int init_y_coord,
             const uint32_t health, const uint32_t damage,
             ItemsContainer& items);

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

    //-----------------------------------------------------------------------------
    //  Movimiento
    //-----------------------------------------------------------------------------

    void startMovingUp();
    void startMovingDown();
    void startMovingRight();
    void startMovingLeft();

    void stopMoving();

    /*
     * Efectúa la recepción del ataque de un character.
     */
    const bool receiveAttack(int& damage, const bool eludible) override;

    /* Retorna la posición de la criatura */
    const Position& getPosition() const override;

    /* Lanza CantRecoverCreaturesHealthException */
    const bool recoverHealth(const unsigned int points) override;

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

    /* No hace nada [por ahora..] */
    void beAttacked();

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
};

class CantRecoverCreaturesHealthException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

#endif  //__CREATURE_H__
