#ifndef __STATES_H__
#define __STATES_H__

/*
 * Interfaz que representa al estado del personaje.
 * Implementan esta interfaz las clases Alive y Dead.
 * Delimita las acciones que se puede realizar en c/estado.
 */
class State {
    public:
        State();
        virtual ~State();

        State(const State&) = delete;
        State& operator=(const State&) = delete;
        State(State&&) = delete;
        State& operator=(State&&) = delete;

        // Definir interfaz comun a Alive/Dead.
        virtual bool canAttack() = 0;
};

/*
 * Representa al estado vivo del personaje.
 * Define las acciones que pueden/no pueden ejecutarse
 * en dicho estado.
 */
class Alive: public State {
    public:
        Alive();
        ~Alive();

        Alive(const Alive&) = delete;
        Alive& operator=(const Alive&) = delete;
        Alive(Alive&&) = delete;
        Alive& operator=(Alive&&) = delete;

        virtual bool canAttack() override;
};

/*
 * Representa al estado muerto del personaje.
 * Define las acciones que pueden/no pueden ejecutarse
 * en dicho estado.
 */
class Dead: public State {
    public:
        Dead();
        ~Dead();

        Dead(const Dead&) = delete;
        Dead& operator=(const Dead&) = delete;
        Dead(Dead&&) = delete;
        Dead& operator=(Dead&&) = delete;

        virtual bool canAttack() override;
};

#endif
