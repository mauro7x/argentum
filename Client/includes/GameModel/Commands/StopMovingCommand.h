#ifndef __STOP_MOVING_COMMAND_H__
#define __STOP_MOVING_COMMAND_H__

//-----------------------------------------------------------------------------
#include "../../../../Common/includes/Protocol.h"
#include "../../../../Common/includes/Socket/SocketWrapper.h"
#include "Command.h"
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class StopMovingCommand : public Command {
   public:
    /* Constructor */
    StopMovingCommand();

    //-------------------------------------------------------------------------

    /* Envío polimórfico del comando. Devuelve si se pudo enviar o no. */
    bool send(const SocketWrapper& socket);

    //-------------------------------------------------------------------------

    /* Destructor */
    ~StopMovingCommand();
};

//-----------------------------------------------------------------------------

#endif  // __STOP_MOVING_COMMAND_H__
