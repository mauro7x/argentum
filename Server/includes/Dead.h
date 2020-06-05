#ifndef __DEAD_H__
#define __DEAD_H__

#include "State.h"

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