#ifndef __START_MOVING_COMMAND_H__
#define __START_MOVING_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
enum MovingDirection { UP_DIR, DOWN_DIR, LEFT_DIR, RIGHT_DIR };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class StartMovingCommand : public Command {
   private:
    MovingDirection dir;

   public:
    /* Constructor */
    StartMovingCommand(MovingDirection dir);

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~StartMovingCommand();
};

//-----------------------------------------------------------------------------

#endif  // __START_MOVING_COMMAND_H__
