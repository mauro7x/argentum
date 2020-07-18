#ifndef __STATES_H__
#define __STATES_H__

#include "../../../Common/includes/DataStructs.h"
#include "Race.h"
#include "config_structs.h"

/*
 * Interfaz que representa al estado del personaje.
 * Implementan esta interfaz las clases Alive, Dead y Resurrecting.
 * Delimita las acciones que se pueden realizar (o no) en c/estado.
 */
class State {
   protected:
    Id head_id;
    Id body_id;

   public:
    State(Id head_id, Id body_id);
    virtual ~State();

    State(const State&) = delete;
    State& operator=(const State&) = delete;
    State(State&&) = delete;
    State& operator=(State&&) = delete;

    virtual const bool attack() const = 0;
    virtual const bool beAttacked() const = 0;
    virtual const bool takeItem() const = 0;
    virtual const bool move() const = 0;
    virtual const bool beHealed() const = 0;
    virtual const bool resurrect() const = 0;
    virtual const bool meditate() const = 0;
    virtual const bool gatherGold() const = 0;

    void fillBroadcastData(PlayerData& data);

    void fillPersistenceData(CharacterCfg& data);
};

class StateFactory {
   public:
    static State* newState(StateType state, const Race& race_data);
};

/*
 * Representa al estado vivo del personaje.
 * Define las acciones que pueden/no pueden ejecutarse
 * en dicho estado.
 */
class Alive : public State {
   public:
    Alive(const Id head_id, const Id body_id);
    ~Alive();

    Alive(const Alive&) = delete;
    Alive& operator=(const Alive&) = delete;
    Alive(Alive&&) = delete;
    Alive& operator=(Alive&&) = delete;

    virtual const bool attack() const override;
    virtual const bool beAttacked() const override;
    virtual const bool takeItem() const override;
    virtual const bool move() const override;
    virtual const bool beHealed() const override;
    virtual const bool resurrect() const override;
    virtual const bool meditate() const override;
    virtual const bool gatherGold() const override;
};

/*
 * Representa al estado muerto del personaje.
 * Define las acciones que pueden/no pueden ejecutarse
 * en dicho estado.
 */
class Dead : public State {
   public:
    Dead(const Id dead_head_id, const Id dead_body_id);
    ~Dead();

    Dead(const Dead&) = delete;
    Dead& operator=(const Dead&) = delete;
    Dead(Dead&&) = delete;
    Dead& operator=(Dead&&) = delete;

    virtual const bool attack() const override;
    virtual const bool beAttacked() const override;
    virtual const bool takeItem() const override;
    virtual const bool move() const override;
    virtual const bool beHealed() const override;
    virtual const bool resurrect() const override;
    virtual const bool meditate() const override;
    virtual const bool gatherGold() const override;
};

class Resurrecting : public State {
   public:
    Resurrecting(const Id dead_head_id, const Id dead_body_id);
    ~Resurrecting();

    Resurrecting(const Resurrecting&) = delete;
    Resurrecting& operator=(const Resurrecting&) = delete;
    Resurrecting(Resurrecting&&) = delete;
    Resurrecting& operator=(Resurrecting&&) = delete;

    virtual const bool attack() const override;
    virtual const bool beAttacked() const override;
    virtual const bool takeItem() const override;
    virtual const bool move() const override;
    virtual const bool beHealed() const override;
    virtual const bool resurrect() const override;
    virtual const bool meditate() const override;
    virtual const bool gatherGold() const override;
};

#endif
