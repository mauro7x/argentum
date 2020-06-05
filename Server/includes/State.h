#ifndef __STATE_H__
#define __STATE_H__

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

#endif