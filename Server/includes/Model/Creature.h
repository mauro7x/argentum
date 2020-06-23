#ifndef __CREATURE_H__
#define __CREATURE_H__

//-----------------------------------------------------------------------------
#include "../../../Common/includes/MapContainer.h"
#include "../../../Common/includes/Orientation.h"
#include "../../../Common/includes/types.h"
#include "../../../Common/includes/UnitData.h"
//-----------------------------------------------------------------------------
#include "config_structs.h"
//-----------------------------------------------------------------------------
#include "Position.h"
//-----------------------------------------------------------------------------

class Creature {
   private:
    Id id;
    std::string name;
    uint32_t health_max;
    uint32_t health_actual;
    uint32_t damage;
    Position position;

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
             const uint32_t health, const uint32_t damage);

    /* Destructor */
    ~Creature();

    /* Deshabilitamos el constructor por copia. */
    Creature(const Creature&) = delete;

    /* Deshabilitamos el operador= para copia.*/
    Creature& operator=(const Creature&) = delete;

    /* Deshabilitamos el constructor por movimiento. */
    Creature(Creature&&) = delete;

    /* Habilitamos el operador= para movimiento. */
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
     * Efectúa la recepción del ataque del jugador.
     * si el damage supera la vida actual,
     */
    void receiveAttack(const unsigned int damage);

    unsigned int getMaxHealth();

    void fillBroadcastData(CreatureData& data) const;

    const bool mustBeBroadcasted() const;
    void beBroadcasted();
};
#endif  //__CREATURE_H__
