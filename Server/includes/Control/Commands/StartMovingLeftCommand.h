#ifndef __START_MOVING_LEFT_COMMAND_H__
#define __START_MOVING_LEFT_COMMAND_H__

#include "Command.h"

// Empezar a moverse a la izquierda.
class StartMovingLeftCommand : public Command {
   public:
    StartMovingLeftCommand(const InstanceId caller);
    ~StartMovingLeftCommand();

    void exec(Game& game) override;
};

#endif  // __START_MOVING_LEFT_COMMAND_H__
