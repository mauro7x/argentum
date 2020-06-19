#ifndef __START_MOVEMENT_COMMAND_H__
#define __START_MOVEMENT_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
enum MovementDirection { UP_DIR, DOWN_DIR, LEFT_DIR, RIGHT_DIR };
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class StartMovementCommand : public Command {
   private:
    MovementDirection dir;

   public:
    /* Constructor */
    StartMovementCommand(MovementDirection dir);

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~StartMovementCommand() {}
};

//-----------------------------------------------------------------------------

#endif  // __START_MOVEMENT_COMMAND_H__
