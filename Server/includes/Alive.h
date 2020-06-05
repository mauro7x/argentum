#ifndef __ALIVE_H__
#define __ALIVE_H__

#include "State.h"

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

#endif