#ifndef __STATES_H__
#define __STATES_H__

#include "../../../Common/includes/DataStructs.h"
#include "Race.h"
#include "config_structs.h"

/*
 * Interfaz que representa al estado del personaje.
 * Implementan esta interfaz las clases Alive y Dead.
 * Delimita las acciones que se puede realizar en c/estado.
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

    virtual void attack() const = 0;
    virtual void beAttacked() const = 0;
    virtual void takeItem() const = 0;

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
    Alive(const Race& race_data);
    ~Alive();

    Alive(const Alive&) = delete;
    Alive& operator=(const Alive&) = delete;
    Alive(Alive&&) = delete;
    Alive& operator=(Alive&&) = delete;

    virtual void attack() const override;
    virtual void beAttacked() const override;
    virtual void takeItem() const override;
};

/*
 * Representa al estado muerto del personaje.
 * Define las acciones que pueden/no pueden ejecutarse
 * en dicho estado.
 */
class Dead : public State {
   public:
    Dead(const Race& race_data);
    ~Dead();

    Dead(const Dead&) = delete;
    Dead& operator=(const Dead&) = delete;
    Dead(Dead&&) = delete;
    Dead& operator=(Dead&&) = delete;

    virtual void attack() const override;
    virtual void beAttacked() const override;
    virtual void takeItem() const override;
};

class AttackedStateCantBeAttackedException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

class AttackerStateCantAttackException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

class StateCantTakeItemException : public std::exception {
   public:
    virtual const char* what() const noexcept;
};

#endif
